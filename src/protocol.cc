#include "stdafx.h"
#include "IProtocol.h"
#ifdef __linux__
#include <string.h>
#include <stdlib.h>
#endif

IProtocol::IProtocol()
{
  m_encodingType = ProtocolTypeHex;
}


IProtocol::~IProtocol()
{

}

void IProtocol::onFrameReceived(const char* frame)
{

}

void IProtocol::addProtocolFormat(const std::string& name, const std::string& format, const std::string& des)
{
  if (name.empty()) return;
  std::map<std::string, ProtocolFormat>::iterator it;
  if ((it=m_strProtocolFormatMap.find(name)) != m_strProtocolFormatMap.end())
  {
    it->second.name = name;
    it->second.format = format;
    it->second.description = des;
  }
  else
  {
    ProtocolFormat protocolFormat;
    protocolFormat.name = name;
    protocolFormat.format = format;
    protocolFormat.description = des;
    m_strProtocolFormatMap.insert(std::pair<std::string, ProtocolFormat>(name, protocolFormat));
  }
}

std::string IProtocol::getProtocolFormat(const std::string& name)
{
  if (name.empty()) return "";
  std::map<std::string, ProtocolFormat>::iterator it;
  if ((it = m_strProtocolFormatMap.find(name)) != m_strProtocolFormatMap.end())
  {
    return it->second.format;
  }
  else
  {
    return "";
  }
}

void IProtocol::setEncodingType(const std::string& type)
{
  if (type.compare("hex") == 0 || type.compare("HEX") == 0)
  {
    this->m_encodingType = ProtocolTypeHex;
  }
  else
  {
    this->m_encodingType = ProtocolTypeAscii;
  }
}

ProtocolEncodingType IProtocol::getEncodingType()
{
  return this->m_encodingType;
}

std::string IProtocol::getEncodingTypeAsString()
{
  if (this->m_encodingType == ProtocolTypeHex)
  {
    return "HEX";
  }
  else
  {
    return "ASCII";
  }
}

int IProtocol::toBytes(std::string& strProtocol, unsigned char* buffer, int length)
{
  int len = strProtocol.length();
  if (len > length) len = length;
  memset(buffer, 0, length);
  std::vector<std::string> charList = split(strProtocol, " ");
  int num = charList.size();
  if (num > length)
  {
    num = length;
  }
  for (int i = 0; i < num; i++)
  {
    buffer[i] = atoi(charList[i].c_str());
  }

  return num;
}

std::string IProtocol::toString(unsigned char* buff, int length)
{
  std::string res = "";
  if (buff == NULL || length == 0) return "";
  for (int i = 0; i < length; i++)
  {
    char d1 = buff[i] / 16;
    char d2 = buff[i] % 16;
    
    char s1[2] = { (d1 < 10) ? d1 + '0' : (d1 - 10) + 'A' , 0 };
    char s2[2] = { (d2 < 10) ? d2 + '0' : (d2 - 10) + 'A' , 0 };
    res += s1;
    res += s2;

    if (i < length - 1) res += " ";
  }

  return res;
}


IMPLEMENT_CLASS_DYNAMIC(IProtocol, ControlObject)

IMPLEMENT_MESSAGE_LIST(IProtocol, ControlObject)

BEGIN_MESSAGE_LIST(IProtocol)
  SET_S(setEncodingType, IProtocol)
  SET_SSS(addProtocolFormat, IProtocol)
END_MESSAGE_LIST
