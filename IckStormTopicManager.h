#ifndef _ICK_STORM_TOPIC_MANAGER_H_
#define _ICK_STORM_TOPIC_MANAGER_H_

////#include <IceStorm/IceStorm.h>
////#include <Ice/Ice.h>
//#include "IckStormTopic.h"
//#include "IObject.h"
//
////
//// 话题（用于发布订阅）管理类
////
//class ick_api IckStormTopicManager : public IceStorm::TopicManager
//{
//public:
//	IckStormTopicManager(Ice::ObjectAdapterPtr adapter, std::string strPublisherEndpoint);
//	~IckStormTopicManager();
//private:
//	IceStorm::TopicDict m_topicDict;
//	Ice::SliceChecksumDict m_sliceChecksumDict;
//	Ice::ObjectAdapterPtr m_adapter;
//	std::string m_strPublisherEndpoint;
//public:
//
//	virtual ::IceStorm::TopicPrx create(const ::std::string&, const ::Ice::Current& = ::Ice::emptyCurrent);
//
//	virtual ::IceStorm::TopicPrx retrieve(const ::std::string&, const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//	virtual ::IceStorm::TopicDict retrieveAll(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//	virtual ::Ice::SliceChecksumDict getSliceChecksums(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//};

#endif
