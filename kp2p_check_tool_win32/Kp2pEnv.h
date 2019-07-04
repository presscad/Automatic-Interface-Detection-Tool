#pragma once

#include "Util.h"
//#include "common.h"
#include <stdio.h>
#include <stdlib.h>


class CKp2pEnv
{
private:
	CKp2pEnv(){}
	CKp2pEnv(const char *id, const char *ip, const char *user, const char *passwd, int func, int channel);
	//DISALLOW_COPY_AND_ASSIGN(CKp2pEnv)
	
public:
	~CKp2pEnv()
	{
		/*DestGlobalCS();
		DestCS(&m_LockCS);*/
	}

public:
	static CKp2pEnv *CreateInstance()
	{
		/*InitGlobalCS();
		InitCS(&m_LockCS);*/
		if (pInstance == NULL) {
			//CSLock lock(m_LockCS);
			//lock(m_mutexInstance);
			if (pInstance == NULL) {
				pInstance = new CKp2pEnv();
			}
			//unlock(m_mutexInstance);
		}
		return pInstance;
	}

	class _FreeInstancee
	{
	public:
		~_FreeInstancee()
		{
			if (CKp2pEnv::pInstance) {
				delete CKp2pEnv::pInstance;
			}
		}
	};
	static _FreeInstancee _free;
	//static CRITICAL_SECTION  m_LockCS;
	//HANDLE  m_Event;

private:
	static CKp2pEnv *pInstance;
	//static pthread_mutex_t m_mutexInstance;

public:
	void Kp2p_context_init();
	//void Kp2p_env_check();
	//void Kp2p_env_check(const char *id, const char *ip, const char *user, const char *passwd, int func, int channel);

};