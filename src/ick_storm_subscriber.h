#ifndef _ICK_STORM_SUBSCRIBER_H_
#define _ICK_STORM_SUBSCRIBER_H_

//#include "IckSubscriber.h"
#include "src/object.h"
#include "src/value_info.h"
#include "src/int_value_info.h"
#include "src/subscriber.h"
#include "src/read_write_int.h"
#include "src/read_only_int.h"
#include <list>

class IckStormSubscriberManager;

struct IckStormValueInfo
{
  AccessMode accessMode;
  DataType   dataType;
  long long  timeStamp;
  int        iValue;
  double     dValue;
  char       sValue[4096];
  // std::string sValue;
};

typedef void(*SubscirberCallBack)(const char* channelName, IckStormValueInfo* valueInfo);



class ick_api IckStormSubscriber /*: public IckCore::IckSubscriber*/
{

public:

  IckStormSubscriber();

  IckStormSubscriber(::std::string channelName, SubscirberCallBack callback = NULL);

  ~IckStormSubscriber();

private:

  std::string m_channelName;

  std::string m_channelIdentity;

  SubscirberCallBack m_callback;

  std::list<Subscriber*> m_subscribers;

public:

  /*
   * @ brief when server publish messages, this function will be called by client
   */
  virtual void update(const std::string& channelName, const ::IckCore::RemoteValueInfo& remoteValueInfo/*, const ::Ice::Current&*/);
  
  std::string getChannelName();
  
  std::string getChannelNameAsIdentity();

};

#endif

