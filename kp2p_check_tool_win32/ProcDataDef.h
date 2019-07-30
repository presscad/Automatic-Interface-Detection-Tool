#pragma once
#include <stdio.h>
#include <stdlib.h>

//本机mac，ip等信息
typedef struct _MyAdpterInfo
{
	std::vector<std::string> Ip;
	std::string MacAddress;
	std::string Description;
	std::string Name;
	UINT Type;
}MyAdpterInfo;

typedef struct MemoryStruct_s
{
	char *memory;
	size_t size;
	MemoryStruct_s()
	{
		memory = (char *)malloc(1);
		size = 0;
	}
	~MemoryStruct_s()
	{
		free(memory);
		memory = NULL;
		size = 0;
	}
}MemoryStruct_t, *pMemoryStruct_t;

//config
typedef struct mem_config_s
{
	int a;
}mem_config_t;

typedef struct cpu_config_s
{
	int a;
}cpu_config_t;

typedef struct process_config_s
{
	int a;
}process_config_t;

typedef struct thread_config_s
{
	int a;
}thread_config_t;


typedef struct kp2p_dev_config_s
{
	char             id[20];
	char             user[20];
	char             password[20];
	char             mtu[10];
	char             gateway[20];
	char             dns[20];
	char             signal[50];
	mem_config_t     mem_use;
	cpu_config_t     cpu_use;
	process_config_t process;
	thread_config_t  thread;
}kp2p_dev_config_t, *pkp2p_dev_config_t;


//status
typedef struct ping_status_s
{
	int a;
}ping_status_t;

typedef struct network_status_s
{
	int a;
}network_status_t;

typedef struct bandwidth_status_s
{
	int a;
}bandwidth_status_t;

typedef struct route_status_s
{
	int a;
}route_status_t;

typedef struct kp2p_dev_status_s
{
	char               id[20];
	char			   user[20];
	char               password[20];
	ping_status_t      ping_info;
	network_status_t   network_info;
	bandwidth_status_t bandwidth_info;
	route_status_t     route_info;
}kp2p_dev_status_t, *pkp2p_dev_status_t;

