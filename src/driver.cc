#include "src/driver.h"

IDriver::IDriver()
{
  m_nodeNumber = 0;
  m_retryTimes = 3;
  m_protocol = NULL;
}


IDriver::~IDriver()
{

}

void IDriver::registChannels()
{
  std::map<std::string,UntypedData*> datas = 
    ObjectManager::getInstance()->getDatas(this->m_nodeNumber);

  std::map<std::string, UntypedData*>::iterator it;
  for (it = datas.begin(); it != datas.end(); it++)
  {
    UntypedData* data = it->second;
    DataType dataType = it->second->getDataType();
    AccessMode accessMode = it->second->getAccessMode();
    FUNC_REG funcRegRead;
    FUNC_REG funcRegWrite;
    if (dataType == INTDATA && accessMode == READONLY)
    {
      funcRegRead.pfn_ri = (bool (CmdTarget::*)(int, int*))&IDriver::registReadInt;
      data->registChannelRead(funcRegRead, this);
    }
    else if (dataType == DOUBLEDATA && accessMode == READONLY)
    {
      funcRegRead.pfn_rd = (bool (CmdTarget::*)(int, double*))&IDriver::registReadDouble;
      data->registChannelRead(funcRegRead, this);
    }
    else if (dataType == STRINGDATA && accessMode == READONLY)
    {
      funcRegRead.pfn_rs = (bool (CmdTarget::*)(int, std::string&))&IDriver::registReadString;
      data->registChannelRead(funcRegRead, this);
    }
    else if (dataType == INTDATA && accessMode == READWRITE)
    {
      funcRegRead.pfn_ri = (bool (CmdTarget::*)(int, int*))&IDriver::registReadInt;
      funcRegWrite.pfn_wi = (bool (CmdTarget::*)(int, int))&IDriver::registWriteInt;
      data->registChannelRead(funcRegRead, this);
      data->registChannelWrite(funcRegWrite, this);
    }
    else if (dataType == DOUBLEDATA && accessMode == READWRITE)
    {
      funcRegRead.pfn_rd = (bool (CmdTarget::*)(int, double*))&IDriver::registReadDouble;
      funcRegWrite.pfn_wd = (bool (CmdTarget::*)(int, double))&IDriver::registWriteDouble;
      data->registChannelRead(funcRegRead, this);
      data->registChannelWrite(funcRegWrite, this);
    }
    
    else if (dataType == STRINGDATA && accessMode == READWRITE)
    {
      funcRegRead.pfn_rs = (bool (CmdTarget::*)(int, std::string&))&IDriver::registReadString;
      funcRegWrite.pfn_ws = (bool (CmdTarget::*)(int, const std::string&))&IDriver::registWriteString;
      data->registChannelRead(funcRegRead, this);
      data->registChannelWrite(funcRegWrite, this);
    }
  }
}

bool IDriver::registReadInt(int channelNumber, int* value)
{
  *value = 0;
  return true;
}

bool IDriver::registReadDouble(int channelNumber, double* value)
{
  *value = 0;
  return true;
}

bool IDriver::registReadString(int channelNumber, std::string& value)
{
  return false;
}

bool IDriver::registWriteInt(int channelNumber, int value)
{
  return true;
}
bool IDriver::registWriteDouble(int channelNumber, double value)
{
  return true;
}
bool IDriver::registWriteString(int channelNumber, const std::string& value)
{
  return true;
}


void IDriver::setRetryTimes(int num)
{
  this->m_retryTimes = num;
}

void IDriver::setNodeNumber(int nodeNumber)
{
    this->m_nodeNumber = nodeNumber;
}

SET(IDriver, setProtocol, m_protocol, IProtocol*)

IMPLEMENT_CLASS_DYNAMIC(IDriver, ControlObject)

IMPLEMENT_MESSAGE_LIST(IDriver, ControlObject)

BEGIN_MESSAGE_LIST(IDriver)
  SET_I(setRetryTimes, IDriver)
  SET_I(setNodeNumber, IDriver)
  SET_S(setProtocol, IDriver)
END_MESSAGE_LIST
