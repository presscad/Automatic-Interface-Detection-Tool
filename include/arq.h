#ifndef __JUAN_ARQ_H_
#define __JUAN_ARQ_H_

#ifdef _WIN32
#ifdef arq_EXPORTS  //cmake will generate this macro/ or manu defined
#define ARQ_API __declspec(dllexport)
#else
#define ARQ_API __declspec(dllimport)
#endif
#else
#define ARQ_API
#endif


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include  "win32int.h"


#ifndef UDP_PROTOCOL
#define UDP_PROTOCOL   ((1<<0))
#endif
#ifndef TCP_PROTOCOL
#define TCP_PROTOCOL   ((1<<1))
#endif

#ifndef IPV4_VER
#define IPV4_VER   4
#endif
#ifndef IPV6_VER
#define IPV6_VER   6
#endif

typedef struct _arq_ipaddr_s {
    u_int8_t ver;
    union {
        u_int32_t ipv4;
        u_int32_t ipv6[4];
    } u;
} ARQ_IPADDR_t;

typedef struct _arq_cb_info_s {
    u_int32_t key;         //通道key
    u_int8_t protocol;    //协议类型
    ARQ_IPADDR_t ip;       //IP地址
    u_int16_t port;        //端口
} ARQ_CB_INFO_t;

typedef struct _arq_stats_s {
    u_int32_t recv_pkts;
    u_int64_t recv_bytes;

    u_int32_t send_pkts;
    u_int64_t send_bytes;
    u_int32_t send_succ_pkts;
    u_int64_t send_succ_bytes;

    u_int32_t socket_recv_pkts;
    u_int64_t socket_recv_bytes;
    u_int32_t socket_send_pkts;
    u_int64_t socket_send_bytes;
} ARQ_STATS_t;

typedef struct _arq_conn_stats_s {
    u_int32_t recv_pkts;
    u_int64_t recv_bytes;

    u_int32_t recv_valid_pkts;
    u_int64_t recv_valid_bytes;

    u_int32_t send_pkts;
    u_int64_t send_bytes;

    u_int32_t send_succ_pkts;
    u_int64_t send_succ_bytes;

    u_int32_t resend_pkts;
    u_int64_t resend_bytes;
} ARQ_CONN_STATS_t;

typedef void ARQ_HANDLE;

/*
*
*@brief 接收回调函数
*@param data    接收到的数据
*@param datalen   数据长度
*@param cb_info   回调信息
*@param user_arg  用户参数
*/
typedef int (*ARQ_RECV_CB)(char *data, int datalen, ARQ_CB_INFO_t *cb_info, void *user_arg);

enum _arq_event_type_e {
    ARQ_EVENT_TYPE_CONN_SUCC = 1,
    ARQ_EVENT_TYPE_CONN_CLOSE,          //关闭连接
    ARQ_EVENT_TYPE_CONN_REMOTE_CLOSE,   //远程关闭连接
    ARQ_EVENT_TYPE_CONN_FAIL,           //连接失败
    ARQ_EVENT_TYPE_CONN_NOFOUND,        //没找到当前连接
    ARQ_EVENT_TYPE_UDP_SOCKET_ERROR,    //UDP发送socket错误
    //6
            ARQ_EVENT_TYPE_SEND_FAIL,           //发送失败
    ARQ_EVENT_TYPE_LOCAL_ADDR,          //本地地址
    ARQ_EVENT_TYPE_CONN_TIMEOUT,        // 连接超时，服务器端默认50秒没收到对端数据认为超时，客户端默认5分钟超时
    ARQ_EVENT_TYPE_UDP_SEND_FAIL,       //发送失败
    ARQ_EVENT_TYPE_SEND_TIMEOUT,        //发送超时， kcp 发送20次认为这包丢了,arq 会再过5秒抛出发送超时
    ARQ_EVENT_TYPE_TCP_SOCKET_ERROR,
    ARQ_EVENT_TYPE_TCP_SEND_FAIL,
    ARQ_EVENT_TYPE_MAX,
    ARQ_EVENT_TYPE_CONN_HAVEINCOMING
};

#define RANDOM_UDP_PORT     (0x4)
#define BLOCK_TCP_SOCKET    (0x2)
#define ARQ_IS_SERVER       (0x1)

/*
*@brief 事件回调函数
*@param  event_type  事件类型
*@param  cb_info   回调信息
*@param  key       接收数据返回的key
*@param  user_arg  用户参数
*/
typedef int (*ARQ_EVENT_CB)(u_int16_t evnent_type, ARQ_CB_INFO_t *cb_info, void *user_arg);

/*
*
*@brief 创建句柄
@param  is_server   创建服务器标识
*@param arq_recv_cb 接收回调函数
*@param arq_error_cb 出错回调函数
*@param user_arg  用户参数
*/
ARQ_API ARQ_HANDLE *
arq_create(u_int8_t is_server, u_int16_t port, ARQ_RECV_CB arq_recv_cb, ARQ_EVENT_CB arq_event_cb, void *user_arg);

/*
*@desc 服务器会同时监听UDP/TCP服务，端口默认是9000
*@param protocol 服务器类型 UDP_SERVER/TCP_SERVER
*@param ipaddr 可选，作为客户端连接时时必须是对端IP
*@param port 可选，作为客户端连接时必须是对端port
*/
//int arq_listen(ARQ_HANDLE *arq, ARQ_IPADDR_t ipaddr, u_int16_t port);

/*
*@brief 清理接口
*@param arg 句柄
*/
ARQ_API void arq_release(ARQ_HANDLE *arq);

/*
*直接发送UDP数据接口
*@para arq
*@para data 发送数据
*@para datalen 发送数据长度
*@param ipaddr 对端IP
*@param port 对端port
*@retval 0 成功 -1 失败
*/

ARQ_API int arq_udp_send(ARQ_HANDLE *arq, char *data, int datalen, ARQ_IPADDR_t ip, u_int16_t port);

/*
*发送接口
*@para arq
*@para key 创建时用的key或arq_recv_cb回调返回的key
*@retval 0 成功 -1 失败
*/
ARQ_API int arq_send(ARQ_HANDLE *arq, u_int32_t key, char *data, int datalen);

/*
* 优先发送接口
* @param arq
* @param key
* @param data
* @param datalen
* @retval 0 成功 -1 失败
*/
ARQ_API int arq_send_priority(ARQ_HANDLE *arq, u_int32_t key, char *data, int datalen);

/*
@brief 设置连接超时
@para arq
@para timeout   连接超时时长，单位为秒
@retver 0 成功 -1 失败
*/
ARQ_API int arq_set_conn_timeout(ARQ_HANDLE *arq, u_int32_t timeout);

/*
*@brief 创建新的连接, 连接时，若UDP在3秒之内没有连接成功会转向TCP连接.
*@param arq arq句柄
*@param ipaddr 对端IP
*@param port 对端port
*@param istcp 是否指定使用TCP连接
*/
ARQ_API int arq_add_conn(ARQ_HANDLE *arq, u_int32_t key, ARQ_IPADDR_t ipaddr, u_int16_t port, uint32_t istcp);

/*
*@brief 使用新的协议创建新的连接, 连接时，若UDP在3秒之内没有连接成功会转向TCP连接.
*@param arq arq句柄
*@param ipaddr 对端IP
*@param port 对端port
*@param istcp 是否指定使用TCP连接
*/
ARQ_API int arq_add_conn_v2(ARQ_HANDLE *arq, u_int32_t key, ARQ_IPADDR_t ipaddr, u_int16_t port, uint32_t istcp);

/*
*@brief       关闭一个连接
*@param  arq  arq句柄
*@param key   连接的key
*
*/
ARQ_API int arq_del_conn(ARQ_HANDLE *arq, u_int32_t key);

/*
*@brief       获取统计数据
*@param  arq_stats 统计
*@retval 0 成功 -1 失败
*
*/
ARQ_API int arq_get_stats(ARQ_STATS_t *arq_stats);

/*
*@brief   获取连接统计数据
*@param  arq  arq句柄
*@param  arq_stats 统计
*@para key 创建时用的key或arq_recv_cb回调返回的key
*@retval 0 成功 -1 失败
*
*/
ARQ_API int arq_get_conn_stats(ARQ_HANDLE *arq, ARQ_CONN_STATS_t *arq_stats, u_int32_t key);

/**
 * @breief 获取 ARQ事件名
 * @param evt 事件
 * @return	事件名
 */
ARQ_API const char *arq_get_event_name(int evt);

ARQ_API void arq_set_zlog(ARQ_HANDLE *arq,void *zlog_cat);

ARQ_API void
arq_get_localaddr(ARQ_HANDLE *arq, ARQ_IPADDR_t *ip, uint16_t *tport, uint16_t *uport);

/**
 * @breief 获取ARQ版本号
 * @return 返回版本号
 */
ARQ_API const char *
arq_getversion(void);

/**
 * @breief 从上次获取后到现在的总流量，每获取一次，流量会重置
 * @param[out] rx_flow 下行总流量
 * @param[out] tx_flow 上行总流量
 * @return 成功返回0
 */
ARQ_API int arq_get_net_flow(ARQ_HANDLE *arq, int32_t *rx_flow, int32_t *tx_flow);

ARQ_API void arq_set_recvbuf_size(int size);

#ifdef __cplusplus
};
#endif


#endif

