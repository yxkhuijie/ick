#include "IckStormTopic.h"


//IckStormTopic::IckStormTopic(Ice::ObjectAdapterPtr m_adapter, std::string strPublisherEndpoint)
//{
//  this->m_adapter = m_adapter;
//  this->m_strPublisherEndpoint = strPublisherEndpoint;
//}
//
//::std::string IckStormTopic::getName(const ::Ice::Current&) const
//{
//  return "";
//};
//
//// ��ȡ������
//// ������Ϊ������20003�˿ڣ���20003�˿�����������߷�����Ϣ
//::Ice::ObjectPrx  IckStormTopic::getPublisher(const ::Ice::Current&) const
//{
//  // return m_adapter->getCommunicator()->stringToProxy("Publisher:default -p 20003");
//  return m_adapter->getCommunicator()->stringToProxy(std::string("Publisher:")+this->m_strPublisherEndpoint);
//};
//
//::Ice::ObjectPrx  IckStormTopic::getNonReplicatedPublisher(const ::Ice::Current&) const {
//  return NULL;
//};
//
//// ���Ľӿں������ͻ�����Ҫ���ô˺���ʵ�ֶ��Ĺ���
//// @ objectPrx:  �ͻ��˶����߶������
//
///**
//* @function   subscribeAndGetPublisher
//* @brief      call this function to subscribe a topic for client
//*
//* ��ʾ���ļ��а�������Shape�Ķ��壬�Լ��˴�ʡȥ��ʮ���֡�
//*
//*/
//::Ice::ObjectPrx  IckStormTopic::subscribeAndGetPublisher(const ::IceStorm::QoS&, const ::Ice::ObjectPrx& objectPrx, const ::Ice::Current&) {
//  // std::cout << "subscribeAndGetPublisher: " << std::endl;
//  // std::cout << "    object id: " << this->m_adapter->getCommunicator()->identityToString(objectPrx->ice_getIdentity()) << std::endl;
//  // std::cout << "    endpoints: " << objectPrx->ice_getEndpoints()[0]->toString() << std::endl;
//  
//
//  IckCore::IckPublisherPrx publisherPrx = IckCore::IckPublisherPrx::checkedCast(this->getPublisher());
//
//  ::IckCore::SubscriberInfo subscriberInfo;
//  subscriberInfo.objectId = ::Ice::identityToString(objectPrx->ice_getIdentity());
//  
//  Ice::EndpointSeq seq = objectPrx->ice_getEndpoints();
//  subscriberInfo.endPoint = seq[0]->toString();
//
//  // IckCore::IckSubscriberPrx subscriberPrx = IckCore::IckSubscriberPrx::checkedCast(objectPrx);
//
//  std::string endpoint = subscriberInfo.objectId + ":" + subscriberInfo.endPoint;
//  std::cout << "data subscriber: endpoint 0: " << endpoint << std::endl;
//  ::std::vector< ::Ice::EndpointPtr>::iterator it;
//  for (it = seq.begin(); it != seq.end(); ++it)
//  {
//    std::cout << "endpoint: " << (*it)->toString();
//  }
//  std::cout << "data subscriber: endpoint: " << endpoint << std::endl;
//  Ice::ObjectPrx prx = publisherPrx->ice_getCommunicator()->stringToProxy(endpoint);
//  // IckCore::IckSubscriberPrx subscriberPrx = IckCore::IckSubscriberPrx::checkedCast(prx);
//
//  // subscriberInfo.channelName = subscriberPrx->getChannelName();
//  publisherPrx->addSubscriber(subscriberInfo);
//
//  return this->getPublisher();
//};
//
//void  IckStormTopic::unsubscribe(const ::Ice::ObjectPrx& objectPrx, const ::Ice::Current&) {
//  IckCore::IckPublisherPrx publisherPrx = IckCore::IckPublisherPrx::checkedCast(this->getPublisher());
//
//  ::IckCore::SubscriberInfo subscriberInfo;
//  subscriberInfo.objectId = ::Ice::identityToString(objectPrx->ice_getIdentity());
//  subscriberInfo.endPoint = objectPrx->ice_getEndpoints()[0]->toString();
//
//  // IckCore::IckSubscriberPrx subscriberPrx = IckCore::IckSubscriberPrx::checkedCast(objectPrx);
//
//  std::string endpoint = subscriberInfo.objectId + ":" + subscriberInfo.endPoint;
//  Ice::ObjectPrx prx = publisherPrx->ice_getCommunicator()->stringToProxy(endpoint);
//
//  publisherPrx->removeSubscriber(subscriberInfo);
//};
//
//void  IckStormTopic::link(const ::IceStorm::TopicPrx&, ::Ice::Int, const ::Ice::Current&) {
//
//};
//
//void  IckStormTopic::unlink(const ::IceStorm::TopicPrx&, const ::Ice::Current&) {
//
//};
//
//::IceStorm::LinkInfoSeq  IckStormTopic::getLinkInfoSeq(const ::Ice::Current&) const {
//  return m_linkInfoSel;
//};
//
//::Ice::IdentitySeq  IckStormTopic::getSubscribers(const ::Ice::Current&) const {
//  return m_identitySeq;
//};
//
//void  IckStormTopic::destroy(const ::Ice::Current&) {
//
//};
//
//void  IckStormTopic::setPublisher(const ::Ice::ObjectPrx objectPrx)
//{
//  this->m_publisher = objectPrx;
//
//}
//
//void IckStormTopic::setPublisherEndpoint(std::string endpoint)
//{
//  this->m_strPublisherEndpoint = endpoint;
//}