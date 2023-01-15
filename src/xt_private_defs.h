/**
 * @file xt_private_defs.h
 * @author tkorays (tkorays@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __XT_PRIVATE_DEFS_H__
#define __XT_PRIVATE_DEFS_H__

#include <stdint.h>
#include <sys/types.h>


#define XT_INTER_STREAM_ID          0xFF
#define XT_STREAM_BUFF_SIZE         1800
// reserve 100 bytes for header
#define XT_STREAM_PAYLOAD_OFFSET    100

typedef enum {
    XT_PKT_TYPE_PADDING = 0,
    XT_PKT_TYPE_STREAM,
    XT_PKT_TYPE_STREAM_RTX,
    XT_PKT_TYPE_STREAM_FEC,

    XT_PKT_TYPE_ACK,
    XT_PKT_TYPE_NACK,
    XT_PKT_TYPE_PINGPONG,
} xt_pkt_type_t;

#pragma pack(1)
/**
 * @brief packet header for xtransport
 * 
 */
typedef struct {
    uint8_t version : 2;     // 版本号，当前为0
    uint8_t connid : 2;      // 连接id，最多支持4个连接同时存在
    uint8_t type : 4;        // 当前包类型

    uint8_t strmid;          // 流id，最多支持256个流，0xFF这个id用于内部通信

    uint8_t dir  : 2;        // 方向
    uint8_t flags1 : 6;
    uint8_t flags2;          // 扩展标志，对于不同的type，含义不同

    uint64_t conn_sn;       // 全局序号，连接层面
    uint32_t conn_ts;        // 发送时间戳
    uint16_t strm_sn;       // 会话层面序号，用户保证会话层面的有序保证
} xt_packet_t;
#pragma pack()

/**
 * @brief xt_context holds all instance for transmission.
 * 
 */
struct xt_context {
    size_t           num_of_connections;
    xt_connection_t*    connections[XT_MAX_CONNECTIONS];
    size_t           num_of_streams;
    xt_stream_t*        streams[XT_MAX_STREAMS];

    clock_t*         clock;
};

/**
 * @brief a connection is a peer to peer transmit channel.
 * 
 */
struct xt_connection {
    xt_conn_id_t        id;
    xt_socket_t*        socket;
    xt_context_t*       context;
    uint64_t         sn;
};

/**
 * @brief data should act as a stream in xtransport.
 * 
 */
struct xt_stream {
    xt_stream_id_t      id;
    xt_stream_dir_t     dir;
    xt_connection_t*    connection1;
    xt_connection_t*    connection2;
    xt_context_t*       context;
    uint8_t*         buffer;
    size_t           buffer_size;
    uint32_t         sn;
};

#ifdef __cplusplus
extern "C" {
#endif

// streams should call this function to send data.
// connection will control the pacing rate and avoid congestion.
int xt_conn_send(xt_connection_t*, void*, int);

int _xt_stream_on_recv(xt_stream_t*, const void*, int);

#ifdef __cplusplus
}
#endif



#endif /* __XT_PRIVATE_DEFS_H__ */
