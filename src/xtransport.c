#include "xtransport.h"
#include "xt_private_defs.h"
#include "cf/socket.h"
#include "cf/memory.h"
#include "cf/bytes.h"
#include "cf/clock.h"
#include <stdio.h>


cf_bool_t xt_ctx_demux_packet(xt_context_t* ctx, const void* buff, int size) {
    xt_packet_t* packet = CF_TYPE_CAST(xt_packet_t*, buff);

    if (size < sizeof(xt_packet_t)) {
        return CF_FALSE;
    }

    if (ctx->streams[packet->strmid] == CF_NULL_PTR) {
        return CF_NULL_PTR;
    }
    printf("recv packet, version: %d, conn: %d, stream_id: %d, %llx\n", packet->version, packet->connid,
        packet->strmid, packet->conn_sn);

    return _xt_stream_on_recv(ctx->streams[packet->strmid], buff, size);
}

xt_context_t* xt_ctx_new() {
    xt_context_t* ctx = CF_NULL_PTR;
    ctx = (xt_context_t*)cf_malloc(sizeof(xt_context_t));
    if (!ctx) {
        return CF_NULL_PTR;
    }

    cf_membzero(CF_TYPE_CAST(void*, ctx), sizeof(xt_context_t));
    ctx->clock = cf_clock_steady();
    return ctx;
}

void xt_ctx_delete(xt_context_t* self) {
    cf_free(self);
}

void xt_ctx_tick_update(xt_context_t* self) {
    int i = 0;
    int recv_bytes = 0;
    cf_uint8_t buff[2000];
    for (i = 0; i < self->num_of_connections; i++) {
        if (self->connections[i]) {
            recv_bytes = self->connections[i]->socket->recv(
                self->connections[i]->socket,
                buff,
                sizeof(buff)
            );
            if (recv_bytes > 0) {
                // demux and deliver to streams
                xt_ctx_demux_packet(self, buff, recv_bytes);
            }
        }
    }
    return;
}

xt_conn_id_t xt_ctx_add_conn(xt_context_t* self, xt_socket_t* socket) {
    xt_connection_t* conn = xt_conn_new(self, socket);
    if (!conn) {
        return -1;
    }
    return conn->id;
}

void xt_ctx_remove_conn(xt_context_t* self, xt_conn_id_t id) {
    xt_conn_delete(self->connections[id]);
}

xt_connection_t* xt_conn_new(xt_context_t* ctx, xt_socket_t* socket) {
    xt_connection_t* conn = CF_NULL_PTR;
    cf_bool_t added = CF_FALSE;
    int i;

    if (ctx->num_of_connections >= XT_MAX_CONNECTIONS) {
        return CF_NULL_PTR;
    }

    conn = (xt_connection_t*)cf_malloc(sizeof(xt_connection_t));
    if (!conn) {
        return CF_NULL_PTR;
    }

    for (i = 0; i < XT_MAX_CONNECTIONS; i++) {
        if(ctx->connections[i] == CF_NULL_PTR) {
            cf_membzero(conn, sizeof(xt_connection_t));
            ctx->num_of_connections++;
            ctx->connections[i] = conn;
            conn->id            = i;
            conn->context       = ctx;
            added = CF_TRUE;
            break;
        }
    }

    if (!added) {
        cf_free(conn);
        return CF_NULL_PTR;
    }

    conn->socket = socket;
    return conn;
}

void xt_conn_delete(xt_connection_t* self) {
    self->context->connections[self->id] = CF_NULL_PTR;
    self->context->num_of_connections--;
    cf_free(self);
}

int xt_conn_send(xt_connection_t* self, void* buff, int size) {
    return self->socket->send(
        self->socket,
        buff,
        size);
}

xt_stream_t* xt_stream_new(xt_context_t* ctx, xt_stream_dir_t dir, int id) {
    xt_stream_t* stream = CF_NULL_PTR;
    cf_uint8_t* buffer = CF_NULL_PTR;
    int i;

    if (id <= XT_STREAM_ID_AUTO || id >= XT_MAX_STREAMS) {
        return CF_NULL_PTR;
    }
    if (id != XT_STREAM_ID_AUTO && ctx->streams[id] != CF_NULL_PTR) {
        return CF_NULL_PTR;
    }

    buffer = cf_malloc(XT_STREAM_BUFF_SIZE);
    if (!buffer) {
        return CF_FALSE;
    }

    if (ctx->num_of_streams >= XT_MAX_STREAMS) {
        return CF_NULL_PTR;
    }
    stream = cf_malloc(sizeof(xt_stream_t));
    if (!stream) {
        return CF_NULL_PTR;
    }

    if (id == XT_STREAM_ID_AUTO) {
        for (i = 0; i < XT_MAX_STREAMS; i++) {
            if (ctx->streams[i] == CF_NULL_PTR) {
                id = i;
                break;
            }
        }
    }
    if (id >= XT_MAX_STREAMS) {
        cf_free(stream);
        cf_free(buffer);
        return CF_NULL_PTR;
    }

    cf_membzero(stream, sizeof(xt_stream_t));
    ctx->num_of_streams++;
    ctx->streams[id] = stream;
    stream->id = id;
    stream->context = ctx;
    stream->dir = dir;
    stream->buffer = buffer;
    stream->buffer_size = XT_STREAM_BUFF_SIZE;

    return stream;
}

cf_bool_t xt_stream_add_conn(xt_stream_t* self, xt_connection_t* conn) {
    if (self->connection1 && self->connection2) {
        return CF_FALSE;
    }
    if (!self->connection1) {
        self->connection1 = conn;
    } else {
        self->connection2 = conn;
    }
    return CF_TRUE;
}

cf_bool_t xt_stream_remove_conn(xt_stream_t* self, xt_connection_t* conn) {
    if (conn == self->connection1) {
        self->connection1 = CF_NULL_PTR;
    }
    if (conn == self->connection2) {
        self->connection2 = CF_NULL_PTR;
    }
    return CF_TRUE;
}

cf_bool_t xt_stream_send(xt_stream_t* self, void* buff, int size) {
    xt_packet_t packet;
    cf_byte_writer_t writer;
    cf_byte_writer_init(&writer, self->buffer, self->buffer_size);

    packet.version  = 0;
    packet.connid   = self->connection1->id;
    packet.type     = XT_PKT_TYPE_STREAM;
    packet.strmid   = self->id;
    packet.dir      = self->dir;
    packet.conn_sn  = self->connection1->sn++;
    packet.conn_ts  = cf_clock_current_ms(self->context->clock);
    packet.strm_sn  = self->sn++;
    printf("con: %d, strm: %d\n", packet.connid, packet.strmid);

    // TODO: 如果需要重传，则需要保存包
    // TODO: FEC
    
    cf_byte_writer_put(&writer, CF_TYPE_CAST(char*, &packet), sizeof(packet));
    cf_byte_writer_put(&writer, buff, size);
    
    int sent_size = xt_conn_send(
        self->connection1,
        cf_byte_writer_data(&writer),
        cf_byte_writer_length(&writer)); 
    return sent_size == cf_byte_writer_length(&writer);
}

cf_bool_t _xt_stream_on_recv(xt_stream_t* self, const void* buff, int size) {
    return CF_TRUE;
}
