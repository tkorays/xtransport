#ifndef __XTRANSPORT_H__
#define __XTRANSPORT_H__

#include "cf/types.h"

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
 * @brief session should do the demuxing work.
 * 
 */
typedef struct xt_session xt_session_t;

/**
 * @brief a real send/recv stream.
 * 
 */
typedef struct xt_stream xt_stream_t;

typedef struct {
    cf_uint32_t version : 2;    // 版本号，当前为0
    cf_uint32_t connid : 2;     // 连接id，最多支持4个连接同时存在
    cf_uint32_t chanid : 8;     // 一个连接上做多承载256个业务
    cf_uint32_t sessid : 8;     // 一个虚拟信道上最多支持256个会话业务
    cf_uint32_t type : 4;       // 当前包类型
    cf_uint32_t flag : 8;       // 符号
    cf_uint32_t seq;            // 全局序号，连接层面
    cf_uint32_t ts;             // 发送时间戳
    cf_uint16_t sess_seq;       // 会话层面序号，用户保证会话层面的有序保证
} xt_packet_t;

typedef int(*xt_connection_send_f)(void* ctx, void*, int);

// 这里需要确保连接已经建立
xt_context_t* xt_context_new();
void xt_context_delete(xt_context_t*);
void xt_context_tick_update(xt_context_t*);

xt_connection_t* xt_connection_new(xt_context_t*);
void xt_connection_delete(xt_connection_t*);
void xt_connection_on_recv(xt_connection_t*, void*, int);

xt_session_t* xt_session_new();
void xt_session_delete(xt_session_t*);
void xt_session_send(xt_session_t*, void*, int);

#endif /* __XTRANSPORT_H__ */
