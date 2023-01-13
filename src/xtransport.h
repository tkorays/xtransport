/**
 * @file xtransport.h
 * @author tkorays (tkorays@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-02
 * 
 * @copyright Copyright (c) 2022 tkorays
 * 
 */
#ifndef __XTRANSPORT_H__
#define __XTRANSPORT_H__

#include "cf/types.h"

CF_DECLS_BEGIN

#define XT_MAX_CONNECTIONS          4
#define XT_MAX_STREAMS              (256)
#define XT_STREAM_ID_AUTO           -1
#define XT_IS_VALID_STREAM_ID(id)   ((id)>= 0 && (id) < XT_MAX_STREAMS)
#define XT_IS_VALID_CONN_ID(id)     ((id) >= 0 and (id) < XT_MAX_CONNECTIONS)

typedef cf_int_t xt_conn_id_t;
typedef cf_int_t xt_stream_id_t;

/**
 * @brief `xt_context_t` is a global transport context.
 * an application should have only one context for all transmission.
 */
typedef struct xt_context xt_context_t;

/**
 * @brief connection is a peer to peer link which represent a transmit channel.
 */
typedef struct xt_connection xt_connection_t;

/**
 * @brief a real send/recv stream.
 * 
 */
typedef struct xt_stream xt_stream_t;

/**
 * @brief encrypte packets
 * 
 */
typedef struct xt_encryptor xt_encryptor_t;

/**
 * @brief decrypte packets
 * 
 */
typedef struct xt_decryptor xt_decryptor_t;

/**
 * @brief determin how a block is splitted to small packets.
 * 
 */
typedef struct xt_packetizer xt_packetizer_t;

/**
 * @brief construct packets into a whole block
 * 
 */
typedef struct xt_depacketizer xt_depacketizer_t;

/**
 * @brief custom nack handler
 * 
 */
typedef struct xt_nack_handler xt_nack_handler_t;

typedef struct {
    int a;
} xt_packet_info_t;

typedef enum {
    XT_STREAM_DIR_NONE = 0,
    XT_STREAM_DIR_SEND,
    XT_STREAM_DIR_RECV,
    XT_STREAM_DIR_BIDI,
} xt_stream_dir_t;

typedef struct xt_socket {
    // pointer of the implemented socket
    void*   ctx;
    // pointer of the peer addr
    void*   peer_addr;
    // send to socket and return sent bytes
    int(*send)(struct xt_socket* self, void* buff, int size);
    // recv from socket and return received bytes
    int(*recv)(struct xt_socket* self, void* buff, int size);
    // set socket's send buffer
    void(*set_send_buffer)(struct xt_socket* self, int size);
    // set socket's recv buffer
    void(*set_recv_buffer)(struct xt_socket* self, int size);
} xt_socket_t;

struct xt_nack_handler {
    void (*XT_NACK_HANDLER_ON_PACKET_FN)(xt_nack_handler_t* self, xt_packet_info_t info);
    cf_bool_t (*XT_NACK_HANDLER_GET_NACKS_FN)(xt_nack_handler_t* self, void*, cf_size_t);
};

xt_context_t* xt_ctx_new();
void xt_ctx_delete(xt_context_t*);
void xt_ctx_tick_update(xt_context_t*);
//xt_conn_id_t xt_ctx_add_conn(xt_context_t*, xt_socket_t*);
//void xt_ctx_remove_conn(xt_context_t*, xt_conn_id_t);
//xt_stream_id_t xt_ctx_add_stream(xt_context_t*, xt_stream_dir_t, xt_stream_id_t);
//void xt_ctx_remove_stream(xt_context_t*, xt_stream_id_t);
//cf_bool_t xt_ctx_send(xt_context_t*, xt_stream_id_t, void*, int);

xt_connection_t* xt_conn_new(xt_context_t*, xt_socket_t*);
void xt_conn_delete(xt_connection_t*);

xt_stream_t* xt_stream_new(xt_context_t*, xt_stream_dir_t, int);
cf_bool_t xt_stream_add_conn(xt_stream_t*, xt_connection_t*);
cf_bool_t xt_stream_remove_conn(xt_stream_t*, xt_connection_t*);
cf_bool_t xt_stream_send(xt_stream_t*, void*, int);


cf_bool_t xt_stream_set_encryptor(xt_stream_t*, xt_encryptor_t*);
cf_bool_t xt_stream_set_decryptor(xt_stream_t*, xt_decryptor_t*);
cf_bool_t xt_stream_set_packetizer(xt_stream_t*, xt_packetizer_t*);
cf_bool_t xt_stream_set_depacktizer(xt_stream_t*, xt_depacketizer_t*);

CF_DECLS_END

#endif /* __XTRANSPORT_H__ */
