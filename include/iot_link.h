//
// Created by 王进 on 23/02/2017.
//
#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#ifdef IOTLink_EXPORTS
#define IOT_LINK_API __declspec(dllexport)
#else
#define IOT_LINK_API __declspec(dllimport)
#endif
#else
#define IOT_LINK_API
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**
 * = 0 成功
 * > 0 正能量的错误
 * < 0 负能量的错误
 *
 * 0  通用成功错误号
 * -1 通用失败错误号
 * -2 参数无效
 *
 * -99 ~ -1 其它模块的一些错误
 *
 *  其它错误号由三位正负数构成：
 *  百位：
 *      1：连接事件 2：设备端 3： 客户端
 *  十位：
 *     0: 常规错误
 *     1：寻址
 *     2：搜索
 *     3：arq 连接
 *     4：设备登陆P2P
 *     5：穿透
 *     6：转发
 *     7: 打开链路
 *
 * 600~ ARQ相关错误号
 * 700~ 链路连接类型 -> OnConnect
 * 800~ 链路断开原因 -> OnClose
 *
 */
enum {

    IOT_LINK_ERR_OK           = 0,
    IOT_LINK_ERR_FAILED       = -1,
    IOT_LINK_ERR_INVALID_ARGS = -2,

    IOT_LINK_ERR_NAT          = -3,
    IOT_LINK_ERR_INVALID_DATA = -4,

    IOT_LINK_ERR_LINK_TYPE_IP_UDP       = 100,
    IOT_LINK_ERR_LINK_TYPE_IP_TCP       = 101,
    IOT_LINK_ERR_LINK_TYPE_HOLE         = 102,
    IOT_LINK_ERR_LINK_TYPE_TURN_UDP     = 103,
    IOT_LINK_ERR_LINK_TYPE_TURN_TCP     = 104,
    IOT_LINK_ERR_LINK_TYPE_P2P_TURN_UDP = 105,
    IOT_LINK_ERR_LINK_TYPE_P2P_TURN_TCP = 106,

    IOT_LINK_ERR_CLOSE         = -100,
    IOT_LINK_ERR_CLOSE_BYPEER  = -101,
    IOT_LINK_ERR_CLOSE_BAD_NET = -102,


    IOT_LINK_ERR_DEV_ONLINE  = 200,
    IOT_LINK_ERR_DEV_OFFLINE = -200,

    IOT_LINK_ERR_DEV_RESOLVE              = -210,
    IOT_LINK_ERR_DEV_RESOLVE_NOT_EXIST_SN = -211,
    IOT_LINK_ERR_DEV_RESOLVE_INVALID_SN   = -212,
    IOT_LINK_ERR_DEV_RESOLVE_DNS          = -213,
    IOT_LINK_ERR_DEV_RESOLVE_JSON         = -214,
    IOT_LINK_ERR_DEV_RESOLVE_SERVER_500   = -215,


    IOT_LINK_ERR_DEV_CONNECT_P2P  = -230,
    IOT_LINK_ERR_DEV_CONNECT_TURN = -231,
    IOT_LINK_ERR_DEV_CONNECT_NGW  = -232,

    IOT_LINK_ERR_DEV_INVALID_NONCE   = -241,
    IOT_LINK_ERR_DEV_REQ_NONCE       = -242,
    IOT_LINK_ERR_DEV_INVALID_SESSION = -243,
    IOT_LINK_ERR_DEV_REQ_SESSION     = -244,
    IOT_LINK_ERR_DEV_LOGIN_P2P       = -245,

    IOT_LINK_ERR_DEV_HOLE_PUNCH = -250,

    IOT_LINK_ERR_DEV_TURN_LOGIN           = -260,
    IOT_LINK_ERR_DEV_TURN_NOT_SUPPORT_P2P = -261,


    IOT_LINK_ERR_DEV_OPEN = -270,


    IOT_LINK_ERR_CLI_RESOLVE               = -310,
    IOT_LINK_ERR_CLI_RESOLVE_DEV_NOT_LOGIN = -311,
    IOT_LINK_ERR_CLI_RESOLVE_DNS           = -312,

    IOT_LINK_ERR_CLI_SEARCH       = -320,
    IOT_LINK_ERR_CLI_SEARCH_JAP2P = -321,

    IOT_LINK_ERR_CLI_CONNECT_P2P      = -330,
    IOT_LINK_ERR_CLI_CONNECT_TURN     = -331,
    IOT_LINK_ERR_CLI_CONNECT_DEV      = -332,
    IOT_LINK_ERR_CLI_CONNECT_NGW      = -333,
    IOT_LINK_ERR_CLI_CONNECT_NOT_KP2P = -334,

    IOT_LINK_ERR_CLI_HOLE_REQ   = -350,
    IOT_LINK_ERR_CLI_HOLE_PUNCH = -351,

    IOT_LINK_ERR_CLI_TURN_REQ           = -360,
    IOT_LINK_ERR_CLI_TURN_LOGIN         = -361,
    IOT_LINK_ERR_CLI_TURN_LOGIN_TIMEOUT = -362,

    IOT_LINK_ERR_CLI_OPEN = -370,

    IOT_LINK_ERR_CLI_INVALID_UID = -380,
    IOT_LINK_ERR_CLI_DEV_OFFLINE = -381,

    /*
     * add by honglee on 2019-04-24
     * check version result
     */
    IOT_LINK_ERR_CLI_CHECKVERSION_FAIL = -390,
    IOT_LINK_ERR_CLI_CHECKVERSION_TIMEOUT = -391
    /*
     * end by honglee on 2019-04-24
     */
};

enum {
    IOT_PeerTypeDevice,
    IOT_PeerTypeApp,
};

/**
 * 链路终端类型
 * device/client/server
 */
#pragma pack(4)
typedef struct iot_peerinfo_s {
    int peertype;
    union {
        struct device {
            /**
             * info 信息的版本
             */
            uint32_t infoversion;
            char     sn[32];
            int      max_ch;
            /**
             * 产品型号，eg:D9304
             */
            char     model[32];
            /**
             * 固件版本
             * 主版本，子版本，修订版本，内部版本
             */
            char     fwversion[32];
            /**
             * 产品硬件码
             */
            char     hwcode[32];
            char     odm[16];
            /**
             * 安装类型， 0， 无， 1， 吊顶， 2， 壁挂
             */
            int      install_type;
            /**
             * 是否支持云存
             */
            int      cloud_record;
            /**
             * 区域码
             */
            char     area[8];
            char     *capabilities;
        } d;
        struct client {
            /**
             * 客户端类型
             */
            char typestr[32];
            char uid[32];
            // ...
        } c;
    }   peer;
} iot_peerinfo_t;


// NOTICE: HOST ByteOrder
typedef struct iot_link_addr_s {
    // family, AF_INET, AF_INET6
    uint32_t af;
    union {
        uint32_t v4;
        uint8_t  v6[16];
    }        ip;
    uint32_t uport;
    uint32_t tport;
} iot_link_addr_t;


// unit: million sec
typedef struct iot_link_conntime_s {
    int total;
    int total_ip;
    int total_hole;
    int total_turn;
    int search_ip;
    int connect_ip;
    int resolve;
    int connect_p2p;
    int request_hole;
    int punch_hole;
    int request_turn;
    int connect_turn;
    int login_turn;
    int open_link;
} iot_link_conntime_t;

typedef struct iot_link_conninfo_s {
    iot_link_addr_t     raddr;
    uint32_t            sid;
    int                 type;
    iot_link_conntime_t time;
} iot_link_conninfo_t;


#define IOT_BANDWIDTH_MAX_SAMPLE_CNT (10)

typedef struct iot_link_bandwidth_stat_info_s {
    // recv
    // Byte/s
    int rx_cur[IOT_BANDWIDTH_MAX_SAMPLE_CNT];  /* 最近的接收带宽统计样本 */
    int rx_avg;
    int rx_max;
    int rx_min;
    int rx_bytes;

    // send/transmit
    // Byte/s
    int tx_cur[IOT_BANDWIDTH_MAX_SAMPLE_CNT]; /* 最近发送带宽统计样本 */
    int tx_avg;
    int tx_max;
    int tx_min;
    int tx_bytes;
} iot_link_bandwidth_stat_info_t;

#pragma pack()


typedef struct iot_link_hooks_s {

    void (*OnConnect)(void *ctx, void *link, iot_link_conninfo_t *info, int ecode,
                      const char *emsg);

    void (*OnClose)(void *ctx, void *link, int ecode, const char *emsg);

    void (*OnError)(void *ctx, void *link, int ecode, const char *emsg);

    void (*OnData)(void *ctx, void *link, const void *data, int len);

    void (*OnBandwidthStatUpdate)(void *ctx, void *link, iot_link_bandwidth_stat_info_t *info,
                                  int ecode,
                                  const char *emsg);

    void (*OnLogUpload)(void *ctx, void *link, const char *log);
} iot_link_hooks_t;


/**
 * 获取当前链路层的版本号。
 * @return
 */
IOT_LINK_API const char *
IOT_GetVersion(void);

/**
 * 初始化链路层，  返回链路层操作句柄 iot
 * @param host  可选，配置绑定地址
 * @param port  可选，配置端口
 * @param info  当前终端信息
 * @param hooks 事件回调
 * @return
 */
IOT_LINK_API void *
IOT_Init(const char *host,
         unsigned short port,
         iot_peerinfo_t *info,
         iot_link_hooks_t *hooks);

/**
 * 销毁链路层
 * @param iiot
 * @return
 */
IOT_LINK_API int
IOT_Deinit(void **iiot);


/**
 * 设置链路层日志等级
 */
enum {
    IOT_LINK_LOG_ALL = (0),
    IOT_LINK_LOG_DEBUG,
    IOT_LINK_LOG_INFO,
    IOT_LINK_LOG_WARN,
    IOT_LINK_LOG_ERROR,
    IOT_LINK_LOG_FATAL,
    IOT_LINK_LOG_NONE,
};


/**
 * @brief  配置日志接口
 */

IOT_LINK_API int
IOT_SetLog(int level, const char *log_path);


/**
 * @brief 链路类型
 */
enum {
    IOT_LINK_LINK_TYPE_NONE         = 0,
    IOT_LINK_LINK_TYPE_IP_UDP       = (1 << 0),
    IOT_LINK_LINK_TYPE_IP_TCP       = (1 << 1),
    IOT_LINK_LINK_TYPE_HOLE         = (1 << 2),
    IOT_LINK_LINK_TYPE_TURN_UDP     = (1 << 3),
    IOT_LINK_LINK_TYPE_TURN_TCP     = (1 << 4),
    IOT_LINK_LINK_TYPE_P2P_TURN_UDP = (1 << 5),
    IOT_LINK_LINK_TYPE_P2P_TURN_TCP = (1 << 6)
};

/**
 * 强制客户端链路连接方式，全局生效
 * @param iot
 * @param type
 * @return
 */
IOT_LINK_API int
IOT_SetLinkType(void *iot, int type);


IOT_LINK_API int
IOT_SetForceTcp(void *iot, bool forcetcp);

IOT_LINK_API int
IOT_SetHole(void *iot, bool enable);

IOT_LINK_API int
IOT_SetP2PSvr(void *iot, const char *p2psvr);

/**
 * 设置链路使用的转发服务器，全局生效, 只适用于客户端
 * @param iot
 * @param addr  eg: 192.168.1.2:9000
 * @return
 */
IOT_LINK_API int
IOT_SetTurnSvr(void *iot, const char *turnsvr);

IOT_LINK_API int
IOT_SetNgw(void *iot, const char *ngw);

IOT_LINK_API int
IOT_UpdatePeerInfo(void *iot, iot_peerinfo_t *info);


typedef struct iot_clientinfo_s {
    char user_name[32];
    char phone_model[32];
    char system_name[32];
    char system_version[32];
    char app_name[32];
    char app_version[32];
    char network_type[32];
    char network_provider[32];
} iot_clientinfo_t;


/**
 * @brief 设置客户端相关信息
 * @param iot
 * @return
 */
IOT_LINK_API int
IOT_SetClientInfo(void *iot, iot_clientinfo_t *info);

/**
 * @brief 设置带宽统计周期，默认为1秒
 */
IOT_LINK_API int
IOT_SetBandwithStatInterval(void *iot, int interval);

IOT_LINK_API int
IOT_GetLastError();

IOT_LINK_API const char *
IOT_Strerror(int ecode);


typedef struct iot_link_search_device_info_s {
    char            uid[32];
    iot_link_addr_t addr;
} iot_search_device_info_t;

typedef void (*iot_on_search_device_f)(iot_search_device_info_t *info, void *ctx);

IOT_LINK_API int
IOT_SearchDeviceStart(void *iot, iot_on_search_device_f on, void *ctx);

IOT_LINK_API int
IOT_SearchDeviceStop(void *iot);


/**
 * 获取链路层头大小
 * @return
 */
IOT_LINK_API int
IOT_GetLinkHdrSize(void);

/**
 * 创建链路句柄，返回成功创建的链路 link
 * @param iot iot 句柄
 * @param ctx  当前链路上下文
 * @return
 *  link 返回创建成功的句柄。
 */
IOT_LINK_API void *
IOT_LINK_Create(void *iot, void *ctx);

/**
 *  创建到对应uid 或者 指定ip 创建链路
 * @param link       链路操作句柄
 * @param uid           设备uid(不是序列号哦)
 * @param svraddr       可选项，填入IP连接优先使用IP, "192.168.1.1:10000" / "[::1]:10000"
 * @param timeout_ms    建立链路超时时间/毫秒
 * @return  返回连接错误码
 *      IOT_LINK_ERR_OK 成功
 *      IOT_LINK_ERR_FAILED 失败
 *      IOT_LINK_ERR_CLI_RESOLVE_DEV_NOT_LOGIN 设备未登陆寻址服务器。
 *      其它错误参考 IOT_LINK_ERR_XX  或者 通过 IOT_LINK_Strerror（ecode） 获取
 *      ....
 */
IOT_LINK_API int
IOT_LINK_Connect(void *link, const char *uid, const char *svraddr, int timeout_ms);

/**
 *  创建到对应uid 的信令链路
 * @param link       链路操作句柄
 * @param uid           设备uid(不是序列号哦)
 * @param timeout_ms    建立链路超时时间/毫秒
 * @return  返回连接错误码
 *      IOT_LINK_ERR_OK 成功
 *      IOT_LINK_ERR_FAILED 失败
 *      IOT_LINK_ERR_CLI_RESOLVE_DEV_NOT_LOGIN 设备未登陆寻址服务器。
 *      其它错误参考 IOT_LINK_ERR_XX  或者 通过 IOT_LINK_Strerror（ecode） 获取
 *      ....
 */
IOT_LINK_API int
IOT_LINK_CMD_Connect(void *link, const char *uid, int timeout_ms);

/**
 *  创建到对应uid 的数据链路
 * @param link       链路操作句柄
 * @param uid           设备uid(不是序列号哦)
 * @param timeout_ms    建立链路超时时间/毫秒
 * @return  返回连接错误码
 *      IOT_LINK_ERR_OK 成功
 *      IOT_LINK_ERR_FAILED 失败
 *      IOT_LINK_ERR_CLI_RESOLVE_DEV_NOT_LOGIN 设备未登陆寻址服务器。
 *      其它错误参考 IOT_LINK_ERR_XX  或者 通过 IOT_LINK_Strerror（ecode） 获取
 *      ....
 */
IOT_LINK_API int
IOT_LINK_DATA_Connect(void *link, const char *uid, int timeout_ms);

/**
 *  关闭信令链路
 * @param llink  链路句柄
 * @return
 */
IOT_LINK_API int
IOT_LINK_CMD_Close(void **link);

/**
 *  关闭数据链路
 * @param llink  链路句柄
 * @return
 */
IOT_LINK_API int
IOT_LINK_DATA_Close(void **link);

/**
 *  关闭链路
 * @param llink  链路句柄
 * @return
 */
IOT_LINK_API int
IOT_LINK_Close(void **llink);


enum {
    IOT_LINK_SEND_PRIOR_NORMAL,
    IOT_LINK_SEND_PRIOR_HIGH,
    IOT_LINK_SEND_PRIOR_IFRAME,
    IOT_LINK_SEND_PRIOR_PFRAME,
    IOT_LINK_SEND_PRIOR_AUDIO,
    IOT_LINK_SEND_PRIOR_VIFRAME,
};

IOT_LINK_API int
IOT_LINK_Send(void *link, const void *data, int len, int prior);

IOT_LINK_API int
IOT_LINK_Send_Cmd(void *link, const void *data, int len);

IOT_LINK_API int
IOT_LINK_Send_Data(void *line, const void *data, int len);


IOT_LINK_API int
IOT_LINK_GetConnInfo(void *link, iot_link_conninfo_t *info);


IOT_LINK_API int
IOT_SetPingPong(void *iot, bool enable);


IOT_LINK_API int
IOT_Reset(void *iot);

IOT_LINK_API int
IOT_LINK_SetShellHooks(void *iot, void *hooks);

#ifdef __cplusplus
}
#endif

