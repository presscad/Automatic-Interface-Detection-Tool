#ifndef _ZLOG_WRAPPER_H
#define _ZLOG_WRAPPER_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "zlog.h"


//========================================================
//  macro definitions
//========================================================
// 日志样式：LF 版本自带换行
// 2017-04-22 15:05:49 | KP2P_Link  | INFO  | IOTLink version 1.0
#define KP2P_ZLOG_FORMAT "\"%d.%ms | %-6c | %-5V | %m\""
#define KP2P_ZLOG_FORMAT_LF "\"%d.%ms | %-6c | %-5V | %m%n\""

// 日志样式： 比普通格式多线程，LF 版本自带换行
// 2017-04-22 15:07:27 | KP2P_Link  | INFO  | 3450:3423144 | IOTLinkCreate version 1.0
#define KP2P_ZLOG_FORMAT_V "\"%d.%ms | %-6c | %-5V | %U %m\""
#define KP2P_ZLOG_FORMAT_V_LF "\"%d.%ms | %-6c | %-5V | %U %m%n\""

// 日志样式： 比普通格式多线程及文件信息，LF 版本自带换行
// 2017-04-22 15:07:27 | KP2P_Link  | INFO  | 3450:3423144 | iot_link.c:45 | IOTLink version 1.0
#define KP2P_ZLOG_FORMAT_VV "\"%d.%ms | %-6c | %-5V | %p:%T | %f:%L | %m\""
#define KP2P_ZLOG_FORMAT_VV_LF "\"%d.%ms | %-6c | %-5V | %p:%T | %f:%L | %m%n\""


// 日志路径
#define KP2P_ZLOG_PATH_STDOUT         "stdout"

//========================================================
//  API declarations
//========================================================
int kp2p_zlog_init();


int kp2p_zlog_deinit();

/**
 * 配置模块日志格式，日志路径
 * @param c 设置日志类目（tag）6个字符对齐， eg: KP2PL /KP2PD / KP2PC/ KP2PT
 * @param c_fmt 日志格式， 请使用宏
 * @param c_log_path 日志路径，默认使用STDOUT，也可指定文件 eg: /tmp/KP2P_Link.log
 * @param c_log_sz  指定日志文件时，日志生成大小 单位为KB
 * @return
 */
int kp2p_zlog_set_category(const char *c, const char *c_fmt, const char *c_log_path, int c_log_sz);

/**
 * 获取对应类目日志句柄
 * @param c
 * @return
 */
zlog_category_t *kp2p_zlog_get_category(const char *c);



#ifdef __cplusplus
}
#endif
#endif
