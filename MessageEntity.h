#ifndef _MESSAGE_ENTITY_H_
#define _MESSAGE_ENTITY_H_

#include "IObject.h"

class CmdTarget;

class MessageEntity : public IObject
{
private:
	std::string m_msg;

	int m_msgType;

	//MethodsMap methodMap;

public:
	MessageEntity();
	~MessageEntity();
	/*
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(bool)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_b = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_i = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_d = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_s = pfn; }

	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, int)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ii = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(double, double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_dd = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, const std::string&)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ss = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_id = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(double, int)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_di = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, const std::string&)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_is = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, int)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_si = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(double, const std::string&)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ds = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_sd = pfn; }

	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, int, int)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_iii = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(double, double, double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ddd = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, const std::string&, const std::string&))
	{
		m_msg = msg; m_msgType = method_type;  methodMap.pfn_sss = pfn;
	}
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, int, double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_iid = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, int, const std::string&)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_iis = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(double, double, const std::string&)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_dds = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, const std::string&, int)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ssi = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(const std::string&, const std::string&, double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ssd = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, double, int)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_idi = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, double, double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_idd = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, double, const std::string&)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_ids = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, const std::string&, int)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_isi = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, const std::string&, double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_isd = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(int, const std::string&, const std::string&)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_iss = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(double, int, int)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_dii = pfn; }
	MessageEntity(std::string msg, int method_type, void (CmdTarget::*pfn)(double, int, double)) { m_msg = msg; m_msgType = method_type;  methodMap.pfn_did = pfn; }
	MessageEntity() { m_msg = ""; m_msgType = METHOD_END;  methodMap.pfn_b = NULL; }
	*/
};

#endif

