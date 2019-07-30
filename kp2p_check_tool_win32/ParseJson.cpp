#include "ParseJson.h"

INT CDevSysInfoObject::parse_sys_info(void *arg)
{
	if (document.Parse(static_cast<char*>(arg)).HasParseError())
		return -1;
	if (!document.IsObject()) {
		return -2;
	}

	if (document.HasMember("id")) {
		if (document["id"].IsString()) {
			id = document["id"].GetString();
		}
	}

	if (document.HasMember("server")) {
		if (document["server"].IsString()) {
			server = document["server"].GetString();
		}
	}
	if (document.HasMember("lasttime")) {
		if (document["lasttime"].IsNumber()) {
			char temp[20] = { 0 };
			time_t tm = document["lasttime"].GetInt();
			//_itoa(document["lasttime"].GetInt(), temp, 10);
			struct tm p;
			p = *localtime(&tm);
			strftime(temp, 1000, "%Y-%m-%d %H:%M:%S", &p);
			last_time = temp;
			//last_time = document["lasttime"].GetDouble();
		}
	}
	if (document.HasMember("firsttime")) {
		if (document["firsttime"].IsString()) {
			char temp[20] = { 0 };
			time_t tm = atoi(document["firsttime"].GetString());
			struct tm p;
			p = *localtime(&tm);
			strftime(temp, 1000, "%Y-%m-%d %H:%M:%S", &p);
			first_time = temp;
			//first_time = document["firsttime"].GetString();
		}
	}
	if (document.HasMember("ch_count")) {
		if (document["ch_count"].IsNumber()) {
			char temp[2] = { 0 };
			_itoa(document["ch_count"].GetInt(), temp, 10);
			ch_count = temp;
			//ch_count = document["chcount"].GetInt();
		}
	}
	if (document.HasMember("hwcode")) {
		if (document["hwcode"].IsString()) {
			hw_code = document["hwcode"].GetString();
		}
	}
	if (document.HasMember("fwversion")) {
		if (document["fwversion"].IsString()) {
			fw_version = document["fwversion"].GetString();
		}
	}
	if (document.HasMember("version")) {
		if (document["version"].IsString()) {
			version = document["version"].GetString();
		}
	}
	if (document.HasMember("odm")) {
		if (document["odm"].IsString()) {
			odm = document["odm"].GetString();
		}
	}
	return 0;
}

INT CDevStatusInfoObject::parse_status_info(void *arg)
{
	if (document.Parse(static_cast<char*>(arg)).HasParseError())
		return -1;
	if (!document.IsObject()) {
		return -1;
	}
	BOOL bOffline = FALSE;
	if (document.HasMember("list")) {
		Value& a = document["list"];
		if (a.IsArray() ) {
			for (SizeType i = 0; i < a.Size(); i++) {
				Value & v = a[i];
				if (!v.IsObject()) {
					return -1;
				}
				if (v.HasMember("status") && v["status"].IsNumber()) {
					char temp[2] = { 0 };
					if (v["status"].GetInt() != 1) {
						bOffline = TRUE;
						status = "设备离线";
					}
					else {
						//_itoa(v["status"].GetInt(), temp, 10);
						status = "设备在线";
					}
					
				}
				if (v.HasMember("nettype") && v["nettype"].IsNumber()) {
					char temp[2] = { 0 };
					int type = v["nettype"].GetInt();
					switch (type) {
					case 0:
						net_type = "完全锥形";
						break;
					case 1:
						net_type = "受限锥形";
						break;
					case 2:
						net_type = "端口受限锥形";
						break;
					case 3:
						net_type = "对称路由";
						break;
					case 255:
						net_type = "未知类型";
						break;
					}
					//_itoa(v["nettype"].GetInt(), temp, 10);
					//net_type = temp;
					//net_type = v["nettype"].GetInt();
				}
				if (v.HasMember("last") && v["last"].IsNumber()) {
					char temp[20] = { 0 };
					time_t tm = v["last"].GetInt();
					struct tm p;
					p = *localtime(&tm);
					strftime(temp, 1000, "%Y-%m-%d %H:%M:%S", &p);
					last_time = temp;
					//_itoa(v["last"].GetInt(), temp, 10);
					//last_time = temp;
					//last_time = v["last"].GetDouble();
				}
				if (v.HasMember("ip") && v["ip"].IsNumber()) {
					char temp[50] = { 0 };
					//char temp1[50] = {0};
					//_itoa(v["ip"].GetInt(), temp, 10);
					int64_t sum = v["ip"].GetInt64();
					//char ip0[4] = { 0 }, ip1[4] = { 0 }, ip2[4] = { 0 }, ip3[4] = {0};
					//int ip0, ip1, ip2, ip3;
					
					/*memcpy(ip0, (char*)&sum, 4);
					memcpy(ip1, (char*)&sum + 4, 4);
					memcpy(ip2, (char*)&sum + 8, 4);
					memcpy(ip3, (char*)&sum + 12, 4);*/

					sprintf(temp, "%lld", sum);
					
					/*memcpy(temp, (char*)&sum, 16);
					memcpy(&ip0, temp, 4);
					memcpy(&ip1, temp + 4, 4);
					memcpy(&ip2, temp + 8, 4);
					memcpy(&ip3, temp + 12, 4);
					memset(temp, '\0', 50);
					sprintf(temp1, "%d:%d:%d:%d", ip0, ip1, ip2, ip3);*/
					//sprintf(temp, "%d:%d:%d:%d", ip0, ip1, ip2, ip3);
					ip = temp;
					//ip = v["ip"].GetDouble();
				}
				if (v.HasMember("port") && v["port"].IsNumber()) {
					char temp[20] = { 0 };
					_itoa(v["port"].GetInt(), temp, 10);
					port = temp;
					//port = v["port"].GetInt();
				}
				if (v.HasMember("proto") && v["proto"].IsNumber()) {
					//char temp[2] = { 0 };
					//_itoa(v["proto"].GetInt(), temp, 10);
					int proto_type = v["proto"].GetInt();
					switch (proto_type) {
					case 2:
						proto = "TCP";
						break;
					}
					
					//proto = v["proto"].GetInt();
				}
				if (v.HasMember("eseeid") && v["eseeid"].IsString()) {
					id = v["eseeid"].GetString();
				}
			}
		}
	}

	if (document.HasMember("count")) {
		if (document["count"].IsNumber()) {
			char temp[2] = { 0 };
			_itoa(document["count"].GetInt(), temp, 10);
			count = temp;
			//count = document["count"].GetInt();
		}
	}
	if (bOffline ) {
		return -1;
	}
	return 0;
}