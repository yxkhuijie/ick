#ifndef _ICK_STORM_MANAGER_
#define _ICK_STORM_MANAGER_

#include "src/object.h"
#include "src/thread.h"
#include <map>
#include <list>
#include "src/subscriber.h"
#include "src/ick_storm_subscriber.h"
#include "src/object_manager.h"

class IckStormSubscriberSocket;
class IckStormServiceSocket;

class ick_api IckStormManager : public Subscriber
{
private:
  
  IckStormManager();

  ~IckStormManager();

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
  static IckStormManager* m_instance;

  IckStormSubscriberSocket* m_subscriberSocket;
  IckStormServiceSocket*    m_serviceSocket;

public:

  static IckStormManager* getInstance();

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

  void setIckStormServiceSocket(IckStormServiceSocket* socket);

  void getNodeInfoList(std::string& nodeName);

  void runService(const std::string& objectName, const std::string& serviceName, const std::string& para);

  /*
   * @ brief when data value changed, whil notify this 
  */
  virtual void update(UntypedData* data);

};

#endif

