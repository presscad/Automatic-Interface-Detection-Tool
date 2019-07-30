#pragma once

#include "Util.h"

class CDevInfoObject
{
public:
	CDevInfoObject(){}
	virtual ~CDevInfoObject(){}

public:
	string id;
	Document document;
};

class CDevSysInfoObject : public CDevInfoObject
{
public:
	CDevSysInfoObject(){}
	~CDevSysInfoObject(){}
	
public:
	string server;
	string last_time;
	string first_time;
	string ch_count;
	string hw_code;
	string fw_version;
	string version;
	string odm;

public:
	INT parse_sys_info(void *arg);
};

class CDevStatusInfoObject : public CDevInfoObject
{
public:
	CDevStatusInfoObject() {}
	~CDevStatusInfoObject() {}

public:
	string status;
	string net_type;
	string last_time;
	string ip;
	string port;
	string proto;
	string count;

public:
	INT parse_status_info(void *arg);
};