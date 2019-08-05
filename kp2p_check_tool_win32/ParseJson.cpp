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
		else if (document["firsttime"].IsNumber()) {
			char temp[20] = { 0 };
			time_t tm = document["firsttime"].GetInt();
			struct tm p;
			p = *localtime(&tm);
			strftime(temp, 1000, "%Y-%m-%d %H:%M:%S", &p);
			first_time = temp;
		}
	}
	if (document.HasMember("ch_count")) {
		if (document["ch_count"].IsNumber()) {
			char temp[2] = { 0 };
			_itoa(document["ch_count"].GetInt(), temp, 10);
			ch_count = temp;
			//ch_count = document["chcount"].GetInt();
		}
		else if (document["ch_count"].IsString()) {
			ch_count = document["ch_count"].GetString();
		}
	}
	if (document.HasMember("hwcode")) {
		if (document["hwcode"].IsString()) {
			hw_code = document["hwcode"].GetString();
		}
	}
	if (document.HasMember("fwversion")) {
		if (document["fwversion"].IsString()) {
			char temp[20] = { 0 };
			//fw_version = document["fwversion"].GetString();
			char *temp_str = const_cast<char*>(document["fwversion"].GetString());
			int v0, v1, v2, v3;
			memcpy(temp, temp_str, 2);
			v0 = atoi(temp);
			memset(temp, 0, 20);

			memcpy(temp, temp_str + 2, 2);
			v1 = atoi(temp);
			memset(temp, 0, 20);

			memcpy(temp, temp_str + 4, 2);
			v2 = atoi(temp);
			memset(temp, 0, 20);

			memcpy(temp, temp_str + 6, 2);
			v3 = atoi(temp);
			memset(temp, 0, 20);

			std::ostringstream ostr;
			ostr << v0 << "." << v1 << "." << v2 << "." << v3;
			fw_version = ostr.str();
		}
	}
	if (document.HasMember("version")) {
		if (document["version"].IsString()) {
			char temp[20] = { 0 };
			//version = document["version"].GetString();
			char *temp_str = const_cast<char*>(document["version"].GetString());
			int v0, v1, v2, v3;
			memcpy(temp, temp_str, 2);
			v0 = atoi(temp);
			memset(temp, 0, 20);

			memcpy(temp, temp_str + 2, 2);
			v1 = atoi(temp);
			memset(temp, 0, 20);

			memcpy(temp, temp_str + 4, 2);
			v2 = atoi(temp);
			memset(temp, 0, 20);

			memcpy(temp, temp_str + 6, 2);
			v3 = atoi(temp);
			memset(temp, 0, 20);
		
			std::ostringstream ostr;
			ostr << v0 << "." << v1 << "." << v2 << "." << v3;
			version = ostr.str();
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
					//_itoa(v["ip"].GetInt(), temp, 10);
					//int64_t ip_int = v["ip"].GetInt64();	
					int ip_int = v["ip"].GetInt();
					sprintf(temp, "%d.%d.%d.%d", ip_int >> 24 & 0xFF, ip_int >> 16 &0xFF, ip_int >> 8 & 0xFF, ip_int & 0xFF);
					ip = temp;
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
					default:
						proto = "未知";
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