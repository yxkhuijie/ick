#include "IckStormSubscriber.h"
#include "TimeConverter.h"
#include "Converter.h"
#include "IckStormSubscriberManager.h"
#include "ReadOnlyDouble.h"

IckStormSubscriber::IckStormSubscriber()
{
  this->m_channelName = "";
  this->m_channelIdentity = "";
  this->m_callback = NULL;
}

IckStormSubscriber::IckStormSubscriber(::std::string channelName, SubscirberCallBack callback)
{
  this->m_channelName = channelName;

  char* name = new char[m_channelName.length() + 1];
  for (int i = 0; i < m_channelName.length(); i++) {
    name[i] = m_channelName[i] == '/' ? '-' : m_channelName[i];
  }
  name[m_channelName.length()] = '\0';
  this->m_channelIdentity = std::string(name);

  this->m_callback = callback;
}

IckStormSubscriber::~IckStormSubscriber()
{

}

void IckStormSubscriber::update(const std::string& channelName, const ::IckCore::RemoteValueInfo& remoteValueInfo/*, const ::Ice::Current&*/)
{
  std::cout << "iValue: " << remoteValueInfo.iValue << ",  dValue: " << remoteValueInfo.dValue << ",  sValue: " << remoteValueInfo.sValue << std::endl;

  std::string value = "";
  IckCore::RemoteAccessMode accessMode = remoteValueInfo.accessMode;
  IckCore::RemoteDataType dataType = remoteValueInfo.dataType;
  
  ValueInfo *valueInfo = NULL;
  UntypedData* untypeData = NULL;

  if (accessMode == IckCore::READONLY && dataType == IckCore::INTDATA)
  {
    value = Converter::convertToString(remoteValueInfo.iValue);
    valueInfo = new IntValueInfo(accessMode, dataType);
    untypeData = new ReadOnlyInt(remoteValueInfo.iValue);
  }

  if (accessMode == IckCore::READONLY && dataType == IckCore::DOUBLEDATA)
  {
    value = Converter::convertToString(remoteValueInfo.dValue);
    valueInfo = new IntValueInfo(accessMode, dataType);
    untypeData = new ReadOnlyDouble(remoteValueInfo.dValue);
  }

  std::list<Subscriber*> subscriberList = IckStormSubscriberManager::getInstance()->getSubscriberList(channelName);
  std::list<Subscriber*>::iterator it;
  for (it = subscriberList.begin(); it != subscriberList.end(); it++)
  {
    if (*it != NULL)
    {
      (*it)->update(untypeData);
    }
  }

  std::list<SubscirberCallBack> callbackList = IckStormSubscriberManager::getInstance()->getSubscriberCallBackList(channelName);
  std::list<SubscirberCallBack>::iterator itCallBack;
  for (itCallBack = callbackList.begin(); itCallBack != callbackList.end(); itCallBack++)
  {
    if (*itCallBack != NULL)
    {
      IckStormValueInfo valueInfo;
      if (remoteValueInfo.accessMode == IckCore::DEFAULT) valueInfo.accessMode = DEFAULT;
      else if (remoteValueInfo.accessMode == IckCore::READONLY) valueInfo.accessMode = READONLY;
      else if (remoteValueInfo.accessMode == IckCore::READWRITE) valueInfo.accessMode = READWRITE;
      if (remoteValueInfo.dataType == IckCore::UNKNOWN) valueInfo.dataType = UNKNOWN;
      else if (remoteValueInfo.dataType == IckCore::INTDATA) valueInfo.dataType = INTDATA;
      else if (remoteValueInfo.dataType == IckCore::DOUBLEDATA) valueInfo.dataType = DOUBLEDATA;
      else if (remoteValueInfo.dataType == IckCore::STRINGDATA) valueInfo.dataType = STRINGDATA;

      valueInfo.iValue = remoteValueInfo.iValue;
      valueInfo.dValue = remoteValueInfo.dValue;
      valueInfo.timeStamp = remoteValueInfo.timeStamp;

      (*itCallBack)(channelName.c_str(),&valueInfo);
    }
  }
  

  TIME_DEBUG(std::string("subscribe from server: channel: ") + channelName + ",  value: " + value + ", subscriber: " + Converter::convertToString((int)subscriberList.size()));
  
}

std::string IckStormSubscriber::getChannelName()
{
  return this->m_channelName;
}

std::string IckStormSubscriber::getChannelNameAsIdentity()
{
  return this->m_channelIdentity;
}
