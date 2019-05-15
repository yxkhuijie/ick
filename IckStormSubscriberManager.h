#ifndef _ICK_STORM_SUBSCRIBER_MANAGER_
#define _ICK_STORM_SUBSCRIBER_MANAGER_

#include "IObject.h"
#include "IThread.h"
#include <map>
#include <list>
//#include <IceStorm/IceStorm.h>
//#include <Ice/Ice.h>
#include "Subscriber.h"
#include "IckStormSubscriber.h"
#include "ObjectManager.h"

class IckStormSubscriberSocket;

class ick_api IckStormSubscriberManager : public Subscriber
{
private:
  
  IckStormSubscriberManager();

  ~IckStormSubscriberManager();

private:

  std::map<std::string, std::list<Subscriber*> > m_subscriberCallBack;
  
  std::map<std::string, IckStormSubscriber*> m_subscribers;
  
  std::map<std::string, std::list<SubscirberCallBack> > m_callbacks;

  
  
  /*IceUtil::Mutex m_mutex;*/
  IMutex m_mutex;

  /*Ice::CommunicatorPtr m_communicator;
  
  Ice::ObjectAdapterPtr m_subscriberAdapter;
  
  IceStorm::TopicManagerPrx m_topicManager;*/

  // unique instance
  static IckStormSubscriberManager* m_instance;

  IckStormSubscriberSocket* m_subscriberSocket;

public:

  static IckStormSubscriberManager* getInstance();

  bool subscriber(const std::string& channelName, IckStormSubscriber* subscirber, SubscirberCallBack callback);

  bool subscriber(IckStormSubscriber* subscirber, SubscirberCallBack callback);

  bool subscriber(IckStormSubscriber* remoteSubscirber, Subscriber* subscriber);
  
  bool subscriber(const std::string& channelName, SubscirberCallBack callback);

  bool subscriber(const std::string& channelName, Subscriber* subscriber);

  bool unsubscriber(const std::string& channelName);

  /*void setSubscriberAdapter(Ice::ObjectAdapterPtr adapter);

  void setTopicManager(IceStorm::TopicManagerPrx topicManager);
  
  IceStorm::TopicManagerPrx getTopicManager();*/

  std::list<Subscriber*> getSubscriberList(const std::string& channelName);

  std::list<SubscirberCallBack> getSubscriberCallBackList(const std::string& channelName);

  void setIckStormSubscriberSocket(IckStormSubscriberSocket* socket);

  void getNodeInfoList(const std::string& nodeName);

  /*
   * @ brief when data value changed, whil notify this 
  */
  virtual void update(UntypedData* data);

};

#endif

