#ifndef _ICK_STORM_PUBLISHER_H_
#define _ICK_STORM_PUBLISHER_H_

//#include "IckPublisher.h"
#include "IckStormSubscriber.h"
#include "IckStormTopic.h"
#include "Subscriber.h"
#include <map>

class ick_api IckStormPublisher : /*public IckCore::IckPublisher, */public Subscriber
{
public:
	IckStormPublisher();
	//IckStormPublisher(/*IceStorm::TopicPrx m_topic*/);
	~IckStormPublisher();

private:

	std::map<std::string, std::list<IckCore::SubscriberInfo> > m_subscribers;
	
	
	IMutex m_mutex;

public:

	/*IceStorm::TopicPrx m_topic;*/

public:

	/*
	 * @ brief publish messages to every client
	 * @ param channelName: channel name value change
	 * @ param valueInfo: value info of data
	 */
	virtual void publish(const std::string& channelName, const IckCore::RemoteValueInfo& valueInfo/*, const ::Ice::Current& = ::Ice::emptyCurrent*/);
	
	/*
	 * @ brief add subscriber client
	 */
	virtual void addSubscriber(const IckCore::SubscriberInfo& subscriberInfo/*, const ::Ice::Current& = ::Ice::emptyCurrent*/);
	
	/*
	 * @ brief remove subscriber client
	 */
	virtual void removeSubscriber(const IckCore::SubscriberInfo& subscriberInfo/*, const ::Ice::Current& = ::Ice::emptyCurrent*/);

	virtual void update(UntypedData* untypedData);
};

#endif

