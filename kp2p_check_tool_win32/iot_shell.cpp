
#include "iot_shell_def.h"
#include "iot_shell.h"
#include "iot_link.h"
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#include <getopt/getopt.h>

#ifndef IOT_SHELL_MAGIC
#define IOT_SHELL_MAGIC (0x494F5453)
#endif

#else

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#endif
#include <stdlib.h>
#include <signal.h>
#include <time.h>


//extern int
//IOT_LINK_SetShellHooks(void *iot, IOT_LINK_ShellHooks *hooks);


struct iot_shell_s {
	bool trigger;
	void                *iot;
	int                 link_hdr_sz;
	void                *link;
	IOT_LINK_ShellHooks hooks;

	// private
	bool                logined;
	void                *ctx;
};

static void
iot_shell_print_prompt(void) {
	//write(STDOUT_FILENO, IOT_SHELL_PROMPT, strlen(IOT_SHELL_PROMPT));
	printf("%s", IOT_SHELL_PROMPT);
}

iot_shell_t *gShell = NULL;

static void
iot_shell_OnShellData(void *ctx, void *session, const void *data, int datasz, int ecode) {
	//    if (!ctx || !session || !data) {
	if (!session || !data) {
		return;
	}

	iot_shell_t     *shell = (iot_shell_t *)ctx;
	iot_shell_hdr_t *hdr = (iot_shell_hdr_t *)data;

	uint32_t magic = IOT_SHELL_MAGIC;
	if (hdr->magic != htonl(magic)) {
		printf("f=shell_OnShellData&invalid_shell_data\n");
		return;
	}

	if (shell->hooks.OnShellData) {
		shell->hooks.OnShellData(shell->ctx, session, (void*)((char*)data + sizeof(iot_shell_hdr_t)),
			datasz - sizeof(iot_shell_hdr_t), hdr->ecode);
		if (hdr->len <= 0) {
			iot_shell_print_prompt();
		}
	}
}


void
iot_shell_OnConnect(void *ctx, void *link, iot_link_conninfo_t *info, int ecode, const char *emsg) {
	    printf("IOTShell connected!\n");
}

void
iot_shell_OnClose(void *ctx, void *link, int ecode, const char *emsg) {
	printf("IOTShell closed!\n");
	if (!ctx) {
		return;
	}
	iot_shell_t *t = (iot_shell_t *)ctx;
	t->trigger = false;
	//exit(EXIT_FAILURE);
}

void
iot_shell_OnError(void *ctx, void *link, int ecode, const char *emsg) {
	printf("IOTShell connected!\n");
}

void
iot_shell_OnData(void *ctx, void *link, const void *data, int datasz) {
	printf("IOTShell connected!\n");
}


int
iot_shell_send_data(void *session, uint32_t cmd, const void *data, int len) {
	if (!session || !data || len < 0) {
		return -1;
	}

	iot_shell_hdr_t *hdr = NULL;

	int  real_datasz = sizeof(*hdr) + len;
	char* buf = (char*)malloc(gShell->link_hdr_sz + real_datasz);
	memset(buf, 0, gShell->link_hdr_sz + real_datasz);
	//memset(buf, 0, sizeof(buf));

	hdr = (iot_shell_hdr_t *)(buf + gShell->link_hdr_sz);
	hdr->magic = htonl(IOT_SHELL_MAGIC);
	hdr->cmd = cmd;
	hdr->len = (uint32_t)len;
	memcpy(hdr->payload, data, len);
	int ret = IOT_LINK_Send(session, hdr, real_datasz, IOT_LINK_SEND_PRIOR_HIGH);
	free(buf);
	return ret;
}

void
signal_catcher(int sig_num) {
	printf("catch a sigint\n");
	char                buf[sizeof(iot_shell_cmd_req_t) + 10];
	iot_shell_cmd_req_t *req = (iot_shell_cmd_req_t *)buf;
	if (gShell) {
		//发送信??
		req->signum = SIGINT;
		req->cmd_len = 4;
		snprintf((char *)req->cmd, 10, "%s", "FUCKU");
		iot_shell_send_data(gShell->link, IOT_SHELL_CMD_REQ, req,
			sizeof(req) + strlen((char *)req->cmd));
	}
	else {
		exit(EXIT_SUCCESS);
	}
}


iot_shell_t *
IOT_SHELL_Init() {
	if (gShell) {
		printf("IOTShell Create already\n");
		//return gShell;
		return NULL;
	}


	gShell = (iot_shell_t *)malloc(sizeof(*gShell));
	if (!gShell) {
		///      printf("IOTShell error: no mem\n");
		return NULL;
	}

	signal(SIGINT, signal_catcher);
	//    signal(SIGQUIT, signal_catcher);

	/*iot_link_hooks_t h = {
		.OnConnect = iot_shell_OnConnect,
		.OnClose = iot_shell_OnClose,
		.OnError = iot_shell_OnError,
		.OnData = iot_shell_OnData
	};*/
	iot_link_hooks_t h;
	h.OnConnect = iot_shell_OnConnect;
	h.OnClose = iot_shell_OnClose;
	h.OnError = iot_shell_OnError;
	h.OnData = iot_shell_OnData;
	h.OnLogUpload = NULL;
	h.OnBandwidthStatUpdate = NULL;

	/*iot_peerinfo_t   info = {
		.peertype = IOT_PeerTypeApp
	};*/
	iot_peerinfo_t   info;
	info.peertype = IOT_PeerTypeApp;
	snprintf(info.peer.c.typestr, sizeof(info.peer.c.typestr), "%s", "IOTShell");

	gShell->iot = IOT_Init(NULL, 0, &info, &h);
	gShell->link_hdr_sz = IOT_GetLinkHdrSize();
	gShell->logined = false;
	IOT_SetForceTcp(gShell->iot, true);

	return gShell;
}


void
IOT_SHELL_Deinit(iot_shell_t **sshell) {
	if (!sshell || !(*sshell)) {
		return;
	}

	iot_shell_t *shell = *sshell;
	IOT_Deinit(&shell->iot);
	*sshell = NULL;
	gShell = NULL;
}

int
IOT_SHELL_SetTurnServer(const char *turnserver) {
	return IOT_SetTurnSvr(gShell->iot, turnserver);
}

void *
IOT_SHELL_Login(iot_shell_t *shell, const char *uid, const char *svraddr,
	IOT_LINK_ShellHooks *hooks, int timeout, void *ctx, int *result) {


	if (!shell) {
		//      printf("IOT_SHELL_Login: invalid shell handle\n");
		return NULL;
	}

	int rt = -1;

	shell->ctx = ctx;

	shell->link = IOT_LINK_Create(shell->iot, shell);

	rt = IOT_LINK_Connect(shell->link, uid, svraddr, timeout);

	//rt = IOT_LINK_Connect(shell->link, "1606129063", svraddr, timeout);
	*result = rt;
	if (0 != rt) {
		//    printf("IOT_SHell_Login: err: connect rt:%d\n", rt);
		IOT_LINK_Close(&shell->link);
		return NULL;
	}

	// 接管应用层的shell 钩子函数
	shell->hooks.OnShellData = hooks->OnShellData;
	/*IOT_LINK_ShellHooks h = {
		.OnShellData = iot_shell_OnShellData
	};*/
	IOT_LINK_ShellHooks h;
	h.OnShellData = iot_shell_OnShellData;

	IOT_LINK_SetShellHooks(shell->iot, &h);


	// login
	iot_shell_login_req_t  *req = NULL;
	iot_shell_login_msg_t  *msg = NULL;
	iot_shell_login_sign_t *sign = NULL;
	int                    msg_len = sizeof(IOT_SHELL_LOGIN_MSG);
	//1024bit wait rsa_lib
	int                    sign_len = 128;
	char*                   buf = (char*)malloc(sizeof(*msg) + msg_len + sizeof(*sign) + sign_len);

	req = (iot_shell_login_req_t *)buf;
	msg = (iot_shell_login_msg_t *)buf;
	sign = (iot_shell_login_sign_t *)buf;

	msg->msg_len = (uint32_t)msg_len;
	sign->sign_len = (uint32_t)sign_len;
	memcpy(msg->msg, IOT_SHELL_LOGIN_MSG, sizeof(IOT_SHELL_LOGIN_MSG));
	memset(sign->sign, 0, sign_len);

	iot_shell_send_data(shell->link, 0, req, sizeof(buf));
	time_t now = time(NULL);
	//    while (time(NULL) - now < 3 && !gShell->logined) {
	//        usleep(5 * 1000);
	//    }

	//    if (gShell->logined) {
	iot_shell_print_prompt();
	return shell->link;
	//    }

	IOT_LINK_Close(&shell->link);
	free(buf);
	return NULL;
}

int
IOT_SHELL_Logout(void **ssession) {
	if (!ssession || !(*ssession)) {
		return -1;
	}

	// logout
	IOT_LINK_Close(ssession);
	*ssession = NULL;
	return 0;
}


int
IOT_SHELL_Send(void *session, const void *data, int data_len, auth_psd_req_data_t *txauth) {
	if (!session || !data) {
		return -1;
	}
	char *cmd = (char *)data;
	if (cmd[0] == '\n') {
		iot_shell_print_prompt();
		return 1;
	}
	char*                buf = (char*)malloc(sizeof(iot_shell_cmd_req_t) + data_len + sizeof(auth_psd_req_data_t));
	memset(buf, 0, sizeof(iot_shell_cmd_req_t) + data_len + sizeof(auth_psd_req_data_t));
	iot_shell_cmd_req_t *req = (iot_shell_cmd_req_t *)buf;

	req->signum = 0;
	req->cmd_len = (uint32_t)data_len;
	memcpy(req->cmd, data, data_len);
	memcpy(req->cmd + data_len, txauth, sizeof(auth_psd_req_data_t));

	//int ret = iot_shell_send_data(session, IOT_SHELL_CMD_REQ, buf, sizeof(buf));
	int ret = iot_shell_send_data(session, IOT_SHELL_CMD_REQ, buf, sizeof(iot_shell_cmd_req_t) + data_len + sizeof(auth_psd_req_data_t));
	//int ret = iot_shell_send_data(session, IOT_SHELL_CMD_REQ, buf, strlen(buf));
	free(buf);
	return ret;
}



