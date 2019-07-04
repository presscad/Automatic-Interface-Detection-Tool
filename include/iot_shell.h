//
// Created by WangJ on 08/05/2017.
// Copyright (c) 2017 JUAN. All rights reserved.
//

#ifndef IOT_IOT_SHELL_H
#define IOT_IOT_SHELL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct iot_shell_s         iot_shell_t;


typedef struct iot_shell_hooks {
    void (*OnShellData)(void *ctx, void *session, const void *data, int datasz,int ecode);
} IOT_LINK_ShellHooks;

typedef struct auth_psd_req_data_s {
uint8_t uid[32];
uint8_t usr[32];
unsigned char pwd[16];
uint8_t mac[32];
uint8_t ver[32];
uint8_t add[32];
uint32_t recode;
}auth_psd_req_data_t;

/**
 * 初始化shell 模块
 * @return
 */
iot_shell_t *
IOT_SHELL_Init();


/**
 * 销毁shell 模块
 * @param sshell
 */
void
IOT_SHELL_Deinit(iot_shell_t **sshell);

int
IOT_SHELL_SetTurnServer(const char *turnserver);

/**
 * 登陆shell
 * 调用此接口需要先初始化链路层
 * @param uid
 * @param verify
 * @param timeout
 * @return
 */

void *
IOT_SHELL_Login(iot_shell_t *shell, const char *uid, const char *verify, IOT_LINK_ShellHooks *hooks,
                int timeout, void *ctx, int *result);



/**
 * 登出
 * @param ssession
 * @return
 */
int
IOT_SHELL_Logout(void **ssession);

/**
 * 发送shell 命令
 * @param session
 * @param data
 * @param data_len
 * @return
 */
int
IOT_SHELL_Send(void *session, const void *data, int data_len,auth_psd_req_data_t *txauth);



#ifdef __cplusplus
}
#endif

#endif //IOT_IOT_SHELL_H
