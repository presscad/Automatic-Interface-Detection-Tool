//
// Created by WangJ on 08/05/2017.
// Copyright (c) 2017 JUAN. All rights reserved.
//
#pragma warning(disable:4200) 
#ifndef KP2PSERVER_IOT_SHELL_DEF_H
#define KP2PSERVER_IOT_SHELL_DEF_H

#include <stdint.h>

#define IOT_SHELL_PROMPT "# "
#define IOT_SHELL_LOGIN_MSG "IOTSHELL"
// 注意数据发送时magic 使用大端序
#define IOT_SHELL_MAGIC (0x494F5453)  // 'IOTS'
//const uint32_t IOT_SHELL_MAGIC = 0x494F5453;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#pragma pack(push,4)
#else
#pragma pack(4)
#endif



enum {
    IOT_SHELL_LOGIN_REQ,
    IOT_SHELL_LOGIN_RES,
    IOT_SHELL_CMD_REQ,
    IOT_SHELL_CMD_RES,
    IOT_SHELL_LOGOUT_REQ,
    IOT_SHELL_LOGOUT_RES,
};

enum {
    IOT_SHELL_ERR_OK,
    IOT_SEHLL_ERR_SIGN, //invalid sign
};
typedef enum err_auth_code_e {
	       ERROR_AUTH_SUCCESS = 0,
		ERROR_AUTH_USR = 1,
		ERROR_AUTH_PWD,
		ERROR_AUTH_MAC,
		ERROR_AUTH_VERSION,
		ERROR_AUTH_ADD,
		ERROR_AUTH_UNKNOW,
		ERROR_AUTH_MAX
}ERROR_AUTH_CODE_E;

typedef struct iot_shell_hdr_s {
    uint32_t magic;
    uint32_t version;
    uint32_t cmd;
    int32_t  ecode;
    uint32_t len;
    char     payload[0];
} iot_shell_hdr_t;


typedef struct iot_shell_login_msg_s {
    uint32_t msg_len;
    char     msg[0];
} iot_shell_login_msg_t;


typedef struct iot_shell_login_sign_s {
    uint32_t sign_len;
    char     sign[0];
} iot_shell_login_sign_t;

// login
typedef struct iot_shell_login_req_s {
    char req[0];
    // data --> msg
    // data + msg_len --> sign
} iot_shell_login_req_t;

// cmd
typedef struct iot_shell_cmd_req_s {
    int32_t  signum;
    uint32_t cmd_len;
    uint8_t  cmd[0];
} iot_shell_cmd_req_t;

// cmd excute result
typedef struct iot_shell_cmd_res_s {
    uint32_t res_len;
    uint8_t  res[0];
} iot_shell_cmd_res_t;


#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif


#ifdef __cplusplus
}
#endif

#endif //KP2PSERVER_IOT_SHELL_DEF_H
