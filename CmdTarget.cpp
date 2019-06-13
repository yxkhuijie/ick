#include "CmdTarget.h"
#include "Converter.h"
#ifdef __linux__
#include <stdlib.h>
#endif

CmdTarget::CmdTarget(void)
{

}


CmdTarget::~CmdTarget(void)
{

}

/**
 * #define BEGIN_MSG_MAP(theClass, baseClass)
 */



const MSG_MAP CmdTarget::m_msgMap =
{                                                             
  NULL,                                                     
  CmdTarget::m_msgEntry
};                                                            
                                                                  
const MSG_MAP* CmdTarget::getMsgMap()
{                                                             
  return &CmdTarget::m_msgMap;
}

void CmdTarget::dispatchMsg(const std::string &msgName, const std::string &params)
{
  bool result = false;
  const MSG_MAP* msgMap = getMsgMap();
  while (msgMap != NULL)
  {
    const MSG_ENTITY* msgEntity = msgMap->m_MsgMapEntity;
    
    while (msgEntity->m_msg.compare("") != 0)
    {
      if (msgEntity->m_msg.compare(msgName) == 0)
      {
        int paramNum = 0;
        if (msgEntity->m_msgType == METHOD_B ||
          msgEntity->m_msgType == METHOD_I ||
          msgEntity->m_msgType == METHOD_D ||
          msgEntity->m_msgType == METHOD_S)
        {
          paramNum = 1;
        }
        else if (
          msgEntity->m_msgType == METHOD_II ||
          msgEntity->m_msgType == METHOD_DD ||
          msgEntity->m_msgType == METHOD_SS ||
          msgEntity->m_msgType == METHOD_ID ||
          msgEntity->m_msgType == METHOD_IS ||
          msgEntity->m_msgType == METHOD_DI ||
          msgEntity->m_msgType == METHOD_DS ||
          msgEntity->m_msgType == METHOD_SI ||
          msgEntity->m_msgType == METHOD_SD)
        {
          paramNum = 2;
        }
        else if (
          msgEntity->m_msgType == METHOD_III ||
          msgEntity->m_msgType == METHOD_DDD ||
          msgEntity->m_msgType == METHOD_SSS ||

          msgEntity->m_msgType == METHOD_IID || 
          msgEntity->m_msgType == METHOD_IIS ||
          msgEntity->m_msgType == METHOD_DDI ||
          msgEntity->m_msgType == METHOD_DDS ||
          msgEntity->m_msgType == METHOD_SSI ||
          msgEntity->m_msgType == METHOD_SSD ||
          
          msgEntity->m_msgType == METHOD_IDD ||
          msgEntity->m_msgType == METHOD_ISS ||
          msgEntity->m_msgType == METHOD_DII ||
          msgEntity->m_msgType == METHOD_DSS ||
          msgEntity->m_msgType == METHOD_SII ||
          msgEntity->m_msgType == METHOD_SDD ||
          
          msgEntity->m_msgType == METHOD_IDI ||
          msgEntity->m_msgType == METHOD_IDS ||
          msgEntity->m_msgType == METHOD_DID ||
          msgEntity->m_msgType == METHOD_DIS ||
          msgEntity->m_msgType == METHOD_SID ||
          msgEntity->m_msgType == METHOD_SIS)
        {
          paramNum = 3;
        }
        else
        {
          std::string msg = std::string("Message dispach failed, msg type not supported! method name: ") + msgName + std::string(", type: ") + Converter::convertToString((int)msgEntity->m_msgType);
          Logger::getInstance()->Error(msg);
          throw msg;
        }

        std::vector<std::string> param = split(params, ",");

        if (param.size() != paramNum)
        {
          std::string msg = std::string("Message dispach failed, param num error! method name: ") + msgName + std::string(", param: ") + params;
          Logger::getInstance()->Error(msg);
          throw msg;
        }

        switch (msgEntity->m_msgType)
        {
        case METHOD_B:
          (this->*msgEntity->methodMap.pfn_b)(param[0] == "true" || param[0] == "1");
          break;
        case METHOD_S:
          (this->*msgEntity->methodMap.pfn_s)(param[0]);
          break;
        case METHOD_I:
          (this->*msgEntity->methodMap.pfn_i)(atoi(param[0].c_str()));
          break;
        case METHOD_SI:
          (this->*msgEntity->methodMap.pfn_si)(param[0],atoi(param[1].c_str()));
          break;
        case METHOD_IS:
          (this->*msgEntity->methodMap.pfn_is)(atoi(param[0].c_str()), param[1]);
          break;
        case METHOD_SS:
          (this->*msgEntity->methodMap.pfn_ss)(param[0], param[1]);
          break;
        case METHOD_II:
          (this->*msgEntity->methodMap.pfn_ii)(atoi(param[0].c_str()), atoi(param[1].c_str()));
          break;
        case METHOD_SSS:
          (this->*msgEntity->methodMap.pfn_sss)(param[0], param[1], param[2]);
          break;
        case METHOD_SSI:
          (this->*msgEntity->methodMap.pfn_ssi)(param[0], param[1], atoi(param[2].c_str()));
          break;
        case METHOD_SIS:
          (this->*msgEntity->methodMap.pfn_sis)(param[0], atoi(param[1].c_str()), param[2]);
          break;
        case METHOD_SDD:
          (this->*msgEntity->methodMap.pfn_sdd)(param[0], atof(param[1].c_str()), atof(param[2].c_str()));
          break;
        case METHOD_SII:
          (this->*msgEntity->methodMap.pfn_sii)(param[0], atoi(param[1].c_str()), atoi(param[2].c_str()));
          break;
        case METHOD_DDD:
          (this->*msgEntity->methodMap.pfn_ddd)(atof(param[0].c_str()), atof(param[1].c_str()), atof(param[2].c_str()));
          break;
        case METHOD_III:
          (this->*msgEntity->methodMap.pfn_iii)(atoi(param[0].c_str()), atoi(param[1].c_str()), atoi(param[2].c_str()));
          break;
        default:
          break;
        }
        result = true;
        return;
      }
      else
      {
        msgEntity++;
      }
    }

    msgMap = msgMap->m_baseMap;
  }
  if (result == false)
  {
    std::string message = this->getFullName() + ": dispatchMsg failed, method name \"" + msgName + "\" not found!";
    Logger::getInstance()->Error(message);
    throw message;
  }
}

std::vector<std::string> CmdTarget::split(std::string str, std::string pattern)
{
  std::string::size_type pos;
  std::vector<std::string> result;

  std::string s = this->trim(str);
  while ((pos = s.find(pattern)) != std::string::npos)
  {
    std::string tmp = s.substr(0, pos);
    // if (tmp != "") result.push_back(tmp);
    result.push_back(tmp);
    s = this->trim(s.substr(pos + 1, s.size() - pos));
  }

  if ((s != "") || (s.compare("") == 0 && str.compare("") != 0))
  {
    result.push_back(s);
  }

  return result;
}

std::string CmdTarget::trim(std::string s)
{
  if (s == "") return "";
  std::string str = s;
  while (str[0] == ' ')
  {
    str = str.substr(1, str.size() - 1);
  }

  while (str[str.size() - 1] == ' ')
  {
    str = str.substr(0, str.size() - 1);
  }

  return str;
}

IMPLEMENT_CLASS_DYNAMIC(CmdTarget, ManagedObject)

BEGIN_MESSAGE_LIST(CmdTarget)
  SET_B(setSimulated, CmdTarget)
END_MESSAGE_LIST


