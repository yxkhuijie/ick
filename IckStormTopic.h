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
//  //
//  ::IceStorm::LinkInfoSeq m_linkInfoSel;
//
//  // ������ID
//  ::Ice::IdentitySeq      m_identitySeq;
//
//  std::string             m_strPublisherEndpoint;
//
//public:
//
//  ::Ice::ObjectPrx m_publisher;
//
//  Ice::ObjectAdapterPtr m_adapter;
//
//public:
//  IckStormTopic(Ice::ObjectAdapterPtr m_adapter, std::string strPublisherEndpoint);
//
//  virtual ::std::string getName(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//  // ��ȡ������
//  // ������Ϊ������20003�˿ڣ���20003�˿�����������߷�����Ϣ
//  virtual ::Ice::ObjectPrx getPublisher(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//  virtual ::Ice::ObjectPrx getNonReplicatedPublisher(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//  // ���Ľӿں������ͻ�����Ҫ���ô˺���ʵ�ֶ��Ĺ���
//  // @ objectPrx:  �ͻ��˶����߶������
//
//  /**   
//   * @function   subscribeAndGetPublisher
//   * @brief      call this function to subscribe a topic for client 
//   *
//   * ��ʾ���ļ��а�������Shape�Ķ��壬�Լ��˴�ʡȥ��ʮ���֡�  
//   *   
//   */
//  virtual ::Ice::ObjectPrx subscribeAndGetPublisher(const ::IceStorm::QoS&, const ::Ice::ObjectPrx& objectPrx, const ::Ice::Current& = ::Ice::emptyCurrent);
//
//  
//    // ȡ������
//  virtual void unsubscribe(const ::Ice::ObjectPrx&, const ::Ice::Current& = ::Ice::emptyCurrent);
//
//  virtual void link(const ::IceStorm::TopicPrx&, ::Ice::Int, const ::Ice::Current& = ::Ice::emptyCurrent);
//
//  virtual void unlink(const ::IceStorm::TopicPrx&, const ::Ice::Current& = ::Ice::emptyCurrent);
//
//  virtual ::IceStorm::LinkInfoSeq getLinkInfoSeq(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//  virtual ::Ice::IdentitySeq getSubscribers(const ::Ice::Current& = ::Ice::emptyCurrent) const;
//
//  virtual void destroy(const ::Ice::Current& = ::Ice::emptyCurrent);
//
//  void setPublisher(const ::Ice::ObjectPrx objectPrx);
//
//  void setPublisherEndpoint(std::string endpoint);
//};

#endif

