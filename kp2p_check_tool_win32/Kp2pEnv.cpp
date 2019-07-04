#include "Kp2pEnv.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
//#include <Windows.h>
#include <getopt/getopt.h>
//#include <Iptypes.h>
//#include <iphlpapi.h>
#else

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/ioctl.h>
#include <net/if.h>

#endif

#include <stdbool.h>

//#include "iot_shell.h"
//#include "iot_shell_def.h"
//#include "md5.h"


CKp2pEnv* CKp2pEnv::pInstance = NULL;
//pthread_mutex_t CKp2pEnv::m_mutexInstance = PTHREAD_MUTEX_INITIALIZER;
//
//static void
//OnConnect(kp2p_handle_t p2p_handle, void *context, const char *conn_type)
//{
//	//    printf("[OnConnect]Handle:%p, type:%s\n", p2p_handle, conn_type);
//	/*conn_times = iclock();
//	conn_status = 1;*/
//}
//
//
//static void
//OnDisconnect(kp2p_handle_t p2p_handle, void *context, int ret)
//{
//	//   printf("[OnDisconnect]Handle:%p disconnected,reason:%d\n", p2p_handle, ret);
//}
//
//
//static void
//OnOpenStream(kp2p_handle_t p2p_handle, void *context, uint32_t channel, uint32_t stream, int err,
//	const char *cam_des)
//{
//	//  printf("OnOpenStream: %s\n", cam_des);
//}
//
//
//static void
//OnCloseStream(kp2p_handle_t p2p_handle, void *context, uint32_t channel, uint32_t stream, int err)
//{
//	//    printf("OnCloseStream: %d %d\n", channel, stream);
//}
//
//
//static void
//dumpH264(const char *path, void *frame, size_t frame_len)
//{
//	/*static FILE *fp264 = NULL;
//	if (!fp264) {
//	fp264 = fopen(path, "w+");
//	}
//	else {
//	fwrite(frame, 1, frame_len, fp264);
//	}*/
//}
//
//
//static FILE *dumpFp[64] = { 0 };
//
//static void
//OnDumpVideo(int ch, const char *enc, void *frame, size_t frame_size)
//{
//	//char dumpname[64] = { 0 };
//	//if (!dumpFp[ch]) {
//	//	snprintf(dumpname, sizeof(dumpname), "ch%d.%s", ch, enc);
//	//	dumpFp[ch] = fopen(dumpname, "w+");
//	//	//     printf("OnDumpVideo: open %s %p\n", dumpname, dumpFp[ch]);
//	//}
//
//	//if (dumpFp[ch]) {
//	//	fwrite(frame, frame_size, 1, dumpFp[ch]);
//	//}
//}
//
//static void
//OnRecvFrameEx(kp2p_handle_t p2p_handle, void *context,
//	uint32_t frame_type,
//	void *frame, size_t frame_size,
//	uint32_t channel,
//	uint64_t time_stamp,
//	uint32_t width,
//	uint32_t height,
//	uint32_t sample_rate,
//	uint32_t sample_width,
//	uint32_t sample_channels,
//	const char *enc)
//{
//
//}
//
//
//static void
//OnVOP2PCallResult(kp2p_handle_t p2p_handle, void *context, int result) {
//	//   printf("OnVOP2PCallResult: %p %d\n", p2p_handle, result);
//}
//
//
//static void
//OnVOP2PHangupResult(kp2p_handle_t p2p_handle, void *context, int result) {
//	//  printf("OnVOP2PHangupResult: %p %d\n", p2p_handle, result);
//}
//
//
//static void
//OnRecvRecFrame(kp2p_handle_t p2p_handle, void *context,
//	uint32_t frame_type,
//	void *frame, size_t frame_size, uint32_t channel, uint64_t time_stamp,
//	uint32_t width, uint32_t height,
//	uint32_t sample_rate, uint32_t sample_width, uint32_t sample_channels,
//	uint32_t gen_time,
//	const char *enc)
//{
//
//}
//
//
//static void
//OnLoginResult(kp2p_handle_t p2p_handle, void *context, int ret)
//{
//	//    printf("OnLoginResult Handle:%p ret:%d\n", p2p_handle, ret);
//	//login_times = iclock();
//}
//
//
//static void
//OnPtzResult(kp2p_handle_t p2p_handle, void *context, int ret)
//{
//	//    printf("OnPtzResult Handle:%p ret:%d\n", p2p_handle, ret);
//}
//
//
//static void
//OnVconRecv(kp2p_handle_t p2p_handle, void *context, uint32_t channel, const void *buf, size_t len)
//{
//
//}
//
//
//static void
//OnRemoteSetup(kp2p_handle_t p2p_handle, void *context, const void *data, size_t data_size)
//{
//	//    printf("%s %d | %s\n", __FUNCTION__, __LINE__, (const char *) data);
//	//setup_res_times = iclock();
//}
//
//
//static void
//OnLogUpload(kp2p_handle_t p2p_handle, void *context, const char *log, size_t log_len)
//{
//	//    printf("OnLogUpload: %s\n", log);
//}

void CKp2pEnv::Kp2p_context_init()
{
	/*kp2p_callback_t cb = { 0 };
	cb.OnConnect = OnConnect;
	cb.OnDisconnect = OnDisconnect;

	cb.OnLoginResult = OnLoginResult;

	cb.OnPtzResult = OnPtzResult;

	cb.OnOpenStream = OnOpenStream;
	cb.OnCloseStream = OnCloseStream;
	cb.OnRecvFrameEx = OnRecvFrameEx;

	cb.OnVOP2PCallResult = OnVOP2PCallResult;
	cb.OnVOP2PHangupResult = OnVOP2PHangupResult;

	cb.OnRecvRecFrame = OnRecvRecFrame;

	cb.OnRemoteSetup = OnRemoteSetup;

	cb.OnVconRecv = OnVconRecv;

	cb.OnP2PError = NULL;


	kp2p_init(&cb);*/
	//Kp2p_env_check();
}

//void CKp2pEnv::Kp2p_env_check()
//{
//}
