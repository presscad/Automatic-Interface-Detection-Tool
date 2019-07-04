#pragma once
#include <stdio.h>
#include <stdlib.h>

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
	char id[20];
	char user[20];
	char password[20];
	char mtu[10];
	char gateway[20];
	char signal[50];
	mem_config_t mem_use;
	cpu_config_t cpu_use;
	process_config_t process;
	thread_config_t thread;
}kp2p_dev_config_t, *pkp2p_dev_config_t;


//status

