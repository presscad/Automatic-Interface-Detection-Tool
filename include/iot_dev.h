//
// Created by WangJ on 06/05/2017.
// Copyright (c) 2017 JUAN. All rights reserved.
//

#ifndef IOT_IOT_DEV_H
#define IOT_IOT_DEV_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


enum {
    IOT_DEV_ERR_OK          = 0,
    IOT_DEV_ERR_FAILED      = -1,
    IOT_DEV_ERR_DEV_ONLINE  = 200,
    IOT_DEV_ERR_DEV_OFFLINE = -200,
    IOT_DEV_ERR_DAEMON_DEAD = -201,

    // NOTICE
    // refer iot_link.h
    // IOT_LINK_ERR_LINK_TYPE_XX
    IOT_DEV_ERR_LINK_TYPE_IP_UDP   = 100,
    IOT_DEV_ERR_LINK_TYPE_IP_TCP   = 101,
    IOT_DEV_ERR_LINK_TYPE_HOLE     = 102,
    IOT_DEV_ERR_LINK_TYPE_TURN_UDP = 103,
    IOT_DEV_ERR_LINK_TYPE_TURN_TCP = 104,

    IOT_DEV_ERR_CLOSE         = -100,
    IOT_DEV_ERR_CLOSE_BYPEER  = -101,
    IOT_DEV_ERR_CLOSE_BAD_NET = -102,
};

typedef struct iot_dev_hooks_s {
    /**
     * NOTICE ctx 可以设置， 用于标识之后链路上产生的事件。
     */
    void (*OnConnect)(void **cctx, void *link, int ecode, const char *emsg);

    void (*OnClose)(void *ctx, void *link, int ecode, const char *emsg);

    void (*OnData)(void *ctx, void *link, const void *data, int len);

    void (*OnError)(void *ctx, void *link, int ecode, const char *emsg);
} iot_dev_hooks_t;


typedef struct iot_dev_info_s {
    /**
     * 序列号
     */
    char sn[32];
    /**
     * 最大通道数
     */
    int  max_ch;
    /**
     * 产品型号、设备类型，eg:P3
     */
    char model[32];
    /**
     * 固件版本
     * 主版本，子版本，修订版本，内部版本
     * A.B.C.D  A,B,C,D 为不大于100的数字
     * 如 1.2.3.4, 若超出100的版本字段只能做截断处理
     * hwcode 就不要瞎鸡八整到这里来了。
     */
    char fwversion[32];
    /**
    * 设备硬件平台代码
    */
    char hwcode[32];
    /**
     * 设备厂商
     */
    char odm[32];
    /**
     * 安装类型， 0， 无(DVR/NVR)， 1， 吊顶， 2， 壁挂
     */
    int  install_type;
    /**
     * 云存录像支持
     */
    int  cloudrecord;
    // TODO, shit definetion
    /**
     *  {"ch_id": 0,"stream": [ 0, 1, 66 ]},
     */
//    char stream_des[32];
    /**
     * 区域码, 国家地区代码，采用ISO 3166标准进行标记，
     * 当不传入该参数时，服务器将根据请求的设备端IP地址，自动侦测所属区域
     */
    char area[8];
} iot_dev_info_t;

/**
 * 创建IOT_DEV
 *  与IOTDaemon 自行通信
 * @param info 指定设备信息
 * @param hooks
 * @return
 */
void *
IOT_DEV_Init(iot_dev_info_t *info, iot_dev_hooks_t *hooks);

/**
 * 销毁IOT_DEV
 * @param ddev
 * @return
 */
int
IOT_DEV_Deinit(void **ddev);


/**
 * @brief 获取IOTDev 版本号(主版本.子版本.补丁版本.微调版本）
 * @return
 */
const char *
IOT_DEV_GetVersion();



enum {
    IOT_DEV_SEND_PRIOR_NORMAL,
    IOT_DEV_SEND_PRIOR_HIGH,
    IOT_DEV_SEND_PRIOR_IFRAME,
    IOT_DEV_SEND_PRIOR_PFRAME,
    IOT_DEV_SEND_PRIOR_AUDIO,
    IOT_DEV_SEND_PRIOR_VIFRAME,
};

int
IOT_DEV_Send(void *link, const void *data, int len, bool prior);

/**
 * 更新设备端相关信息
 * @param dev
 * @param info
 * @return
 */
int
IOT_DEV_UpdateInfo(void *dev, iot_dev_info_t *info);

/**
 * @brief 设置日志路径
 * @param level
 * @param path
 * @return
 */

int
IOT_DEV_SetLog(int level, const char *path);


#ifdef __cplusplus
}
#endif

#endif //IOT_IOT_DEV_H
