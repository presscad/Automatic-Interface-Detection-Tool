#ifndef _PROTO_H
#define _PROTO_H
#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64)
#define ALIGNED(n) __declspec(align(n))
#else
#define ALIGNED(n) __attribute__((aligned(n)))
#endif

#if defined(__MACH__) || defined(__unix__)
#include <sys/types.h>
#endif
#include <stdint.h>

//========================================================
//  macro definitions
//========================================================
/* 协议幻数 "KP2P" */
#define APP_PROTO_MAGIC                 0x4b503250
#define APP_PROTO_VERSION               1


/* 协议命令 */
#define APP_PROTO_CMD_AUTH_REQ          10
#define APP_PROTO_CMD_AUTH_RSP          11

#define APP_PROTO_CMD_NONCE_REQ         120
#define APP_PROTO_CMD_NONCE_RSP         121

#define APP_PROTO_CMD_AUTH2_REQ         130
#define APP_PROTO_CMD_AUTH2_RSP         131

#define APP_PROTO_CMD_AUTH3_REQ         140
#define APP_PROTO_CMD_AUTH3_RSP         141

#define APP_PROTO_CMD_PTZ_REQ           20
#define APP_PROTO_CMD_PTZ_RSP           21

#define APP_PROTO_CMD_LIVE_REQ          30
#define APP_PROTO_CMD_LIVE_RSP          31

#define APP_PROTO_CMD_FIND_START_REQ    90
#define APP_PROTO_CMD_FIND_START_RSP    91

#define APP_PROTO_CMD_FIND_NEXT_REQ     100
#define APP_PROTO_CMD_FIND_NEXT_RSP     101

#define APP_PROTO_CMD_FIND_STOP_REQ     110
#define APP_PROTO_CMD_FIND_STOP_RSP     111

#define APP_PROTO_CMD_REPLAY_REQ        40
#define APP_PROTO_CMD_REPLAY_RSP        41

#define APP_PROTO_CMD_VCON_REQ          50
#define APP_PROTO_CMD_VCON_RSP          51

#define APP_PROTO_CMD_VOP2P_REQ         60
#define APP_PROTO_CMD_VOP2P_RSP         61

#define APP_PROTO_CMD_ALARM_REQ         70
#define APP_PROTO_CMD_ALARM_RSP         71

#define APP_PROTO_CMD_SETUP_REQ         80
#define APP_PROTO_CMD_SETUP_RSP         81

#define APP_PROTO_CMD_SETUP2_REQ        150
#define APP_PROTO_CMD_SETUP2_RSP        151

#define APP_PROTO_CMD_BANDWIDTH         160


/* LIVE子命令 */
#define APP_PROTO_PARAM_LIVE_CMD_STOP           1
#define APP_PROTO_PARAM_LIVE_CMD_START          2
#define APP_PROTO_PARAM_LIVE_CMD_CHANGE_STREAM  3


/* REPLAY子命令 */
#define APP_PROTO_PARAM_REPLAY_CMD_SEARCH       1
#define APP_PROTO_PARAM_REPLAY_CMD_STOP         2
#define APP_PROTO_PARAM_REPLAY_CMD_START        3
#define APP_PROTO_PARAM_REPLAY_CMD_PAUSE        4
#define APP_PROTO_PARAM_REPLAY_CMD_CONTINUE     5


/* 录像查找子命令 */
#define APP_PROTO_DATA_FIND_FILE_START          1
#define APP_PROTO_DATA_FIND_FILE_NEXT           2
#define APP_PROTO_DATA_FIND_FILE_STOP           3


/* VCON子命令 */
#define APP_PROTO_PARAM_VCON_CMD_CREATE         1
#define APP_PROTO_PARAM_VCON_CMD_DATA           2
#define APP_PROTO_PARAM_VCON_CMD_DESTROY        3
#define APP_PROTO_PARAM_VCON_APP_NAME_STRLEN    32


/* 语音子命令 */
#define APP_PROTO_PARAM_VOP2P_CMD_CALL          1
#define APP_PROTO_PARAM_VOP2P_CMD_HANGUP        2


/* 字符参数长度 */
#define APP_PROTO_PARAM_AUTH_NAME_STRLEN        32
#define APP_PROTO_PARAM_AUTH_PASSWD_STRLEN      32

#define APP_PROTO_PARAM_AUTH_NAME_STRLEN2       1024
#define APP_PROTO_PARAM_AUTH_PASSWD_STRLEN2     1024

#define APP_PROTO_PARAM_NONCE_STRLEN            64

#define APP_PROTO_PARAM_AUTH_INFO_STRLEN        64
#define APP_PROTO_PARAM_SIGNATURE_STRLEN        64

#define APP_PROTO_PARAM_LIVE_CAM_DESC_STRLEN    32

#define APP_PROTO_PARAM_SETUP_STRLEN            2048


/* AES128加密密钥 */
#define APP_PROTO_AES128_KEY                    "~!JUAN*&Vision-="

/* 命令执行错误码 */
#define APP_PROTO_SUCCESS                   0
#define APP_PROTO_AUTH_FAILED               -11 /* 用户验证失败 */
#define APP_PROTO_OPEN_STREAM_FAILED        -21 /* 打开码流失败 */
#define APP_PROTO_CLOSE_STREAM_FAILED       -22 /* 关闭码流失败 */
#define APP_PROTO_VOP2P_CALL_FAILED         -31 /* 语音拨打失败 */
#define APP_PROTO_VOP2P_HANGUP_FAILED       -32 /* 语音挂起失败 */
#define APP_PROTO_PTZ_CONTROL_FAILED        -41 /* 云台控制失败 */
#define APP_PROTO_FIND_START_FAILED         -51 /* 开始录像查找失败 */
#define APP_PROTO_FIND_NEXT_FAILED          -52 /* 查找下一条失败 */
#define APP_PROTO_FIND_STOP_FAILED          -53 /* 结束查找失败 */
#define APP_PROTO_REPLAY_SEARCH_FAILED      -61 /* 开始录像搜索失败 */
#define APP_PROTO_REPLAY_STOP_FAILED        -71 /* 结束录像播放失败 */
#define APP_PROTO_REPLAY_START_FAILED       -72 /* 开始录像播放失败 */
#define APP_PROTO_REPLAY_PAUSE_FAILED       -73 /* 暂停录像播放失败 */
#define APP_PROTO_REPLAY_RESUME_FAILED      -74 /* 继续录像播放失败 */
#define APP_PROTO_REMOTE_SETUP_FAILED       -81 /* 远程控制失败 */

#define IOT_PROTO_MULTICHANNEL_VERSION 2

/* 初始化快捷方式 */
#define app_proto_header_initializer(s, _tick, _cmd, _size) \
{ \
    s.head.magic = APP_PROTO_MAGIC; \
    s.head.version = APP_PROTO_VERSION; \
    s.head.tick = _tick; \
    s.head.cmd = _cmd; \
    s.head.size = _size; \
}


#define app_proto_header_pointer_initializer(p, _tick, _cmd, _size) \
{ \
    p->head.magic = APP_PROTO_MAGIC; \
    p->head.version = APP_PROTO_VERSION; \
    p->head.tick = _tick; \
    p->head.cmd = _cmd; \
    p->head.size = _size; \
}


//========================================================
//  structure defintions
//========================================================
/* 协议头 */
typedef struct app_proto_head_s {
    /* 协议幻数 */
    uint32_t magic;
    /* 协议版本 */
    uint32_t version;
    /* 协议流水号 */
    uint32_t tick;
    /* 协议命令 */
    uint32_t cmd;
    /* 协议执行结果, 根据协议不同有不同的值 */
    int32_t result;
    /* 具体协议结构体大小: 如sizeof(auth_req_data_t) */
    uint32_t size;
} ALIGNED(4) app_proto_head_t;


/* 用户校验协议 */
typedef struct auth_req_data_s {
    /* AES128加密的用户名 */
    uint8_t encrypt_name[APP_PROTO_PARAM_AUTH_NAME_STRLEN];
    /* AES128加密的密码 */
    uint8_t encrypt_passwd[APP_PROTO_PARAM_AUTH_PASSWD_STRLEN];
} ALIGNED(4) auth_req_data_t;

typedef struct auth_req_s {
    app_proto_head_t head;
    auth_req_data_t data;
} ALIGNED(4) auth_req_t;

typedef struct auth_rsp_data_s {
    uint32_t reserve;
} ALIGNED(4) auth_rsp_data_t;

typedef struct auth_rsp_s {
    app_proto_head_t head;
    auth_rsp_data_t data;
} ALIGNED(4) auth_rsp_t;


/* 用户校验协议加长版 */
typedef struct auth3_req_data_s {
    /* 用户名 */
    char name[APP_PROTO_PARAM_AUTH_NAME_STRLEN2];
    /* 密码 */
    char passwd[APP_PROTO_PARAM_AUTH_PASSWD_STRLEN2];
} ALIGNED(4) auth3_req_data_t;

typedef struct auth3_req_s {
    app_proto_head_t head;
    auth3_req_data_t data;
} ALIGNED(4) auth3_req_t;

typedef struct auth3_rsp_data_s {
    uint8_t reserve[1024];
} ALIGNED(4) auth3_rsp_data_t;

typedef struct auth3_rsp_s {
    app_proto_head_t head;
    auth3_rsp_data_t data;
} ALIGNED(4) auth3_rsp_t;


/* 独占模式获取nonce */
typedef struct nonce_req_data_s {
    uint32_t reserve;
} ALIGNED(4) nonce_req_data_t;

typedef struct nonce_req_s {
    app_proto_head_t head;
    nonce_req_data_t data;
} ALIGNED(4) nonce_req_t;

typedef struct nonce_rsp_data_s {
    uint8_t nonce[APP_PROTO_PARAM_NONCE_STRLEN];
} ALIGNED(4) nonce_rsp_data_t;

typedef struct nonce_rsp_s {
    app_proto_head_t head;
    nonce_rsp_data_t data;
} ALIGNED(4) nonce_rsp_t;


/* 独占模式校验协议 */
typedef struct auth2_req_data_s {
    uint8_t auth_info[APP_PROTO_PARAM_AUTH_INFO_STRLEN];
    uint8_t signature[APP_PROTO_PARAM_SIGNATURE_STRLEN];
} ALIGNED(4) auth2_req_data_t;

typedef struct auth2_req_s {
    app_proto_head_t head;
    auth2_req_data_t data;
} ALIGNED(4) auth2_req_t;

typedef struct auth2_rsp_data_s {
    uint32_t reserve;
} ALIGNED(4) auth2_rsp_data_t;

typedef struct auth2_rsp_s {
    app_proto_head_t head;
    auth2_rsp_data_t data;
} ALIGNED(4) auth2_rsp_t;


/* 预览协议 */
typedef struct live_req_data_s {
    /* 通道 */
    uint32_t channel;
    /* 码流 */
    uint32_t stream;
    /* 预览命令*/
    uint32_t live_cmd;
} ALIGNED(4) live_req_data_t;

typedef struct live_req_s {
    app_proto_head_t head;
    live_req_data_t data;
} ALIGNED(4) live_req_t;

typedef struct live_rsp_data_s {
    /* 通道 */
    uint32_t channel;
    /* 码流 */
    uint32_t stream;
    /* 预览命令*/
    uint32_t live_cmd;
    /* 通道描述 */
    char cam_desc[APP_PROTO_PARAM_LIVE_CAM_DESC_STRLEN];
} ALIGNED(4) live_rsp_data_t;

typedef struct live_rsp_s {
    app_proto_head_t head;
    live_rsp_data_t data;
} ALIGNED(4) live_rsp_t;


/* 语音对讲协议 */
typedef struct vop2p_req_data_s {
    /* 语音对讲命令 */
    uint32_t vop2p_cmd;
    /* 语音对讲通道 */
    uint32_t channel;
} ALIGNED(4) vop2p_req_data_t;

typedef struct vop2p_req_s {
    app_proto_head_t head;
    vop2p_req_data_t data;
} ALIGNED(4) vop2p_req_t;

typedef struct vop2p_rsp_data_s {
    /* 语音对讲命令 */
    uint32_t vop2p_cmd;
    /* 语音对讲通道 */
    uint32_t channel;
} ALIGNED(4) vop2p_rsp_data_t;

typedef struct vop2p_rsp_s {
    app_proto_head_t head;
    vop2p_rsp_data_t data;
} ALIGNED(4) vop2p_rsp_t;


/* 云台控制协议 */
typedef struct ptz_req_data_s {
    /* 通道号 */
    uint32_t channel;
    /* 动作 */
    uint32_t action;
    /* 参数1 表示云台运动速度或者镜头参数级别 */
    uint32_t param1;
    /* aux开关 */
    uint32_t param2;
} ALIGNED(4) ptz_req_data_t;

typedef struct ptz_req_s {
    app_proto_head_t head;
    ptz_req_data_t data;
} ALIGNED(4) ptz_req_t;

typedef struct ptz_rsp_data_s {
    uint32_t reserve;
} ALIGNED(4) ptz_rsp_data_t;

typedef struct ptz_rsp_s {
    app_proto_head_t head;
    ptz_rsp_data_t data;
} ALIGNED(4) ptz_rsp_t;


/* 时间结构 */
typedef struct date_time_s {
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
} ALIGNED(4) date_time_t;


/* 录像查找协议 */
/* 开始 */
typedef struct find_start_req_data_s {
    /* 查找通道 */
    uint32_t channel;
    /* 录像类型 */
    uint32_t file_type;
    /* 开始时间 */
    date_time_t start_time;
    /* 结束时间 */
    date_time_t stop_time;
} ALIGNED(4) find_start_req_data_t;

typedef struct find_start_req_s {
    app_proto_head_t head;
    find_start_req_data_t data;
} ALIGNED(4) find_start_req_t;

typedef struct find_start_rsp_data_s {
    uint32_t reserve;
} ALIGNED(4) find_start_rsp_data_t;

typedef struct find_start_rsp_s {
    app_proto_head_t head;
    find_start_rsp_data_t data;
} ALIGNED(4) find_start_rsp_t;

/* 查找下一条记录 */
typedef struct find_next_req_data_s {
    uint32_t reserve;
} ALIGNED(4) find_next_req_data_t;

typedef struct find_next_req_s {
    app_proto_head_t head;
    find_next_req_data_t data;
} ALIGNED(4) find_next_req_t;

typedef struct find_next_rsp_data_s {
    uint32_t channel;
    uint32_t file_type;
    uint32_t file_size;
    date_time_t start_time;
    date_time_t stop_time;
} ALIGNED(4) find_next_rsp_data_t;

typedef struct find_next_rsp_s {
    app_proto_head_t head;
    find_next_rsp_data_t data;
} ALIGNED(4) find_next_rsp_t;

/* 查找关闭 */
typedef struct find_stop_req_data_s {
    uint32_t reserve;
} ALIGNED(4) find_stop_req_data_t;

typedef struct find_stop_req_s {
    app_proto_head_t head;
    find_stop_req_data_t data;
} ALIGNED(4) find_stop_req_t;

typedef struct find_stop_rsp_data_s {
    uint32_t reserve;
} ALIGNED(4) find_stop_rsp_data_t;

typedef struct find_stop_rsp_s {
    app_proto_head_t head;
    find_stop_rsp_data_t data;
} ALIGNED(4) find_stop_rsp_t;


/* 回放协议 */
typedef struct replay_req_data_s {
    /* 回放命令 */
    uint32_t replay_cmd;
    /* 回放打开类型 */
    uint32_t open_type;
    /* 通道数 位或方式表示*/
    uint32_t channels[4];
    /* 录像类型 */
    uint32_t type;
    /* 日期: 高16位表示年,低8-16位表示月,低8位表示日 */
    uint32_t date;
    /* 开始时间 */
    uint32_t begin_time;
    /* 结束时间 */
    uint32_t end_time;
    /* 录像质量 */
    uint32_t quality;
    /* 记录索引 */
    uint32_t session_index;
    /* 请求记录数 */
    uint32_t session_count;
} ALIGNED(4) replay_req_data_t;

typedef struct replay_req_s {
    app_proto_head_t head;
    replay_req_data_t data;
} ALIGNED(4) replay_req_t;

typedef struct replay_rsp_data_s {
    /* 回放命令 */
    uint32_t replay_cmd;
    /* 通道数 位或方式表示*/
    uint32_t channels[4];
    /* 录像类型 位或方式表示 */
    uint32_t type;
    /* 日期: 高16位表示年,低8-16位表示月,低8位表示日 */
    uint32_t date;
    /* 开始时间 */
    uint32_t begin_time;
    /* 结束时间 */
    uint32_t end_time;
    /* 录像质量 */
    uint32_t quality;
    /* 记录索引 */
    uint32_t session_index;
    /* 请求记录数 */
    uint32_t session_count;
    /* 总记录数 */
    uint32_t session_total;
} ALIGNED(4) replay_rsp_data_t;

typedef struct replay_rsp_s {
    app_proto_head_t head;
    replay_rsp_data_t data;
} ALIGNED(4) replay_rsp_t;


/* 远程设置 */
typedef struct setup_req_data_s {
    uint32_t req_size;
    uint8_t req_str[APP_PROTO_PARAM_SETUP_STRLEN];
} ALIGNED(4) setup_req_data_t;

typedef struct setup_req_s {
    app_proto_head_t head;
    setup_req_data_t data;
} ALIGNED(4) setup_req_t;

typedef struct setup_rsp_data_s {
    uint32_t rsp_size;
    uint8_t rsp_str[APP_PROTO_PARAM_SETUP_STRLEN];
} ALIGNED(4) setup_rsp_data_t;

typedef struct setup_rsp_s {
    app_proto_head_t head;
    setup_rsp_data_t data;
} ALIGNED(4) setup_rsp_t;


typedef struct setup2_req_data_s {
    uint8_t reserve[32];
    uint32_t req_size;
} ALIGNED(4) setup2_req_data_t;

typedef struct setup2_req_s {
    app_proto_head_t head;
    setup2_req_data_t data;
} ALIGNED(4) setup2_req_t;

typedef struct setup2_rsp_data_s {
    uint8_t reserve[32];
    uint32_t rsp_size;
} ALIGNED(4) setup2_rsp_data_t;

typedef struct setup2_rsp_s {
    app_proto_head_t head;
    setup2_rsp_data_t data;
} ALIGNED(4) setup2_rsp_t;


/* 虚拟通道 */
typedef struct vcon_req_data_s {
    uint32_t channel;
    uint8_t reserve[32];
    uint32_t data_size;
} ALIGNED(4) vcon_req_data_t;

typedef struct vcon_req_s {
    app_proto_head_t head;
    vcon_req_data_t data;
} ALIGNED(4) vcon_req_t;

typedef vcon_req_data_t vcon_rsp_data_t;

typedef struct vcon_rsp_s {
    app_proto_head_t head;
    vcon_rsp_data_t data;
} ALIGNED(4) vcon_rsp_t;


/* 带宽统计 */
typedef struct bandwidth_req_data_s {
    /* 带宽数据(单位0.01KB/S) */
    uint32_t bandwidth;
    uint8_t reserve[32];
} ALIGNED(4) bandwidth_req_data_t;

typedef struct bandwidth_req_s {
    app_proto_head_t head;
    bandwidth_req_data_t data;
} ALIGNED(4) bandwidth_req_t;


#ifdef __cplusplus
}
#endif
#endif
