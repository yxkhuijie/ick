#ifndef _ICK_STORM_TOPIC_H_
#define _ICK_STORM_TOPIC_H_

//#include <IceStorm\IceStorm.h>
//#include <Ice\Ice.h>
//#include "IckPublisher.h"
//#include "IckSubscriber.h"
//#include "IObject.h"
//
//class ick_api IckStormTopic : public IceStorm::Topic
//{
//private:
//
//	//
//	::IceStorm::LinkInfoSeq m_linkInfoSel;
//
//	// 订阅者ID
//	::Ice::IdentitySeq      m_identitySeq;
//
//	std::string             m_strPublisherEndpoint;
//
//public:
//
//	::Ice::ObjectPrx m_publisher;
//
//	Ice::ObjectAdapterPtr m_adapter;
//
//public:
//	IckStormTopic(Ice::ObjectAdapterPtr m_adapter, std::string strPublisherEndpoint);
//
//	virtual ::std::string getName(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//	// 获取发布者
//	// 发布者为本机的20003端口，由20003端口向各个订阅者发布消息
//	virtual ::Ice::ObjectPrx getPublisher(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//	virtual ::Ice::ObjectPrx getNonReplicatedPublisher(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//	// 订阅接口函数，客户端需要调用此函数实现订阅功能
//	// @ objectPrx:  客户端订阅者对象代理
//
//	/**   
//	 * @function   subscribeAndGetPublisher
//	 * @brief      call this function to subscribe a topic for client 
//	 *
//	 * 本示例文件中包含了类Shape的定义，以及此处省去的十余字…  
//	 *   
//	 */
//	virtual ::Ice::ObjectPrx subscribeAndGetPublisher(const ::IceStorm::QoS&, const ::Ice::ObjectPrx& objectPrx, const ::Ice::Current& = ::Ice::emptyCurrent);
//
//	
//    // 取消订阅
//	virtual void unsubscribe(const ::Ice::ObjectPrx&, const ::Ice::Current& = ::Ice::emptyCurrent);
//
//	virtual void link(const ::IceStorm::TopicPrx&, ::Ice::Int, const ::Ice::Current& = ::Ice::emptyCurrent);
//
//	virtual void unlink(const ::IceStorm::TopicPrx&, const ::Ice::Current& = ::Ice::emptyCurrent);
//
//	virtual ::IceStorm::LinkInfoSeq getLinkInfoSeq(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//	virtual ::Ice::IdentitySeq getSubscribers(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//	virtual void destroy(const ::Ice::Current& = ::Ice::emptyCurrent);
//
//	void setPublisher(const ::Ice::ObjectPrx objectPrx);
//
//	void setPublisherEndpoint(std::string endpoint);
//};

#endif

