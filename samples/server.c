#include "xtransport.h"
#include <stdio.h>

#if 0
int socket_send(struct xt_socket* self, void* buff, int size) {
    return cf_socket_sendto(CF_TYPE_CAST(cf_socket_t*, self->ctx), CF_TYPE_CAST(cf_sockaddr_t*, self->peer_addr), buff, size);
}
int socket_recv(struct xt_socket* self, void* buff, int size) {
    return cf_socket_recvfrom(CF_TYPE_CAST(cf_socket_t*, self->ctx), CF_TYPE_CAST(cf_sockaddr_t*, self->peer_addr), buff, size);

}
void socket_set_sendbuf(void* ctx, int size) {

}
void socket_set_recvbuf(void* ctx, int size) {

}

int main(int argc, char** argv) {
    cf_socket_t socket;
    cf_sockaddr_t local_addr;
    cf_sockaddr_t remote_addr;
    xt_socket_t xt_socket;
    char buff[2000];
    xt_context_t* ctx = xt_ctx_new();
    xt_connection_t* conn = xt_conn_new(ctx, &xt_socket);
    xt_stream_t* strm_send = xt_stream_new(ctx, XT_STREAM_DIR_SEND, 5);
    xt_stream_t* strm_send1 = xt_stream_new(ctx, XT_STREAM_DIR_SEND, 6);

    cf_socket_bootstrap();
    cf_socket_init(&socket, CF_TRUE, CF_FALSE);
    cf_socket_make_ipv4_addr(&local_addr, 192, 168, 1, 111, 5060);
    cf_socket_make_ipv4_addr(&remote_addr, 192, 168, 1, 111, 5070);
    cf_socket_bind(&socket, &local_addr);

    xt_socket.ctx = &socket;
    xt_socket.peer_addr = &remote_addr;
    xt_socket.send = socket_send;
    xt_socket.recv = socket_recv;
    xt_socket.set_send_buffer = socket_set_sendbuf;
    xt_socket.set_recv_buffer = socket_set_recvbuf;


    //xt_conn_id_t connid = xt_ctx_add_conn(ctx, &xt_socket);

    xt_stream_add_conn(strm_send, conn);
    xt_stream_add_conn(strm_send1, conn);
    while (CF_TRUE) {
        xt_stream_send(strm_send, buff, 20);
        xt_stream_send(strm_send1, buff, 20);
        printf("send packet\n");
        cf_time_sleep(500);
    }

    cf_socket_shutdown();
    return 0;
}
#endif

int main(int argc, char *argv[])
{
    return 0;
}
