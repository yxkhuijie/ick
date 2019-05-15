#include "IckStormTopicManager.h"


//
//IckStormTopicManager::IckStormTopicManager(Ice::ObjectAdapterPtr adapter, std::string strPublisherEndpoint)
//{
//  m_adapter = adapter;
//  this->m_strPublisherEndpoint = strPublisherEndpoint;
//}
//
//IckStormTopicManager::~IckStormTopicManager()
//{
//
//}
//
//
//// @brief
//// 创建一个新的话题
//IceStorm::TopicPrx IckStormTopicManager::create(const std::string& name, const ::Ice::Current&)
//{
//  // IceInternal::ProxyHandle<IckStormTopic> topicPrx(new IckStormTopic());
//  //IckStormTopic* topicPtr = new IckStormTopic();
//  IceStorm::TopicPtr topicPtr = new IckStormTopic(this->m_adapter, m_strPublisherEndpoint);
//  //IceInternal::ProxyHandle<IckStormTopic> topicPrx1 = IceInternal::ProxyHandle<IckStormTopic>::checkedCast(m_adapter->addFacetWithUUID(topicPtr, name));
//  IceStorm::TopicPrx topicPrx = IceStorm::TopicPrx::checkedCast(m_adapter->addWithUUID(topicPtr));
//
//  //IceStorm::TopicPrx topicPrx;
//  //topicPrx._ptr = topicPtr;
//  // IceInternal::ProxyHandle<IckStormTopic> topicPrx = dynamic_cast<IckStormTopic*>(topicPtr._ptr);
//  // topicPrx._ptr = new IckStormTopic();
//  this->m_topicDict.insert(std::pair<std::string, IceStorm::TopicPrx>(name, topicPrx));
//
//
//  return topicPrx;
//}
//IceStorm::TopicPrx IckStormTopicManager::retrieve(const std::string& name, const ::Ice::Current&) const
//{
//  IceStorm::TopicDict::const_iterator it = this->m_topicDict.find(name);
//  if ((it = this->m_topicDict.find(name)) != this->m_topicDict.end()) {
//    return it->second;
//  }
//  return NULL;
//}
//IceStorm::TopicDict IckStormTopicManager::retrieveAll(const ::Ice::Current&) const
//{
//  return this->m_topicDict;
//}
//
//::Ice::SliceChecksumDict IckStormTopicManager::getSliceChecksums(const ::Ice::Current&) const
//{
//  return m_sliceChecksumDict;
//}
