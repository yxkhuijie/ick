#include "IckStormPublisher.h"
#include "ObjectManager.h"
#include "ReadOnlyDouble.h"


IckStormPublisher::IckStormPublisher()
{
}
//
//IckStormPublisher::IckStormPublisher(IceStorm::TopicPrx m_topic)
//{
//	this->m_topic = m_topic;
//}

IckStormPublisher::~IckStormPublisher()
{
}

void IckStormPublisher::publish(const std::string& channelName, const IckCore::RemoteValueInfo& valueInfoPtr/*, const ::Ice::Current&*/)
{
	std::string::size_type pos = 0;
	char* name = new char[channelName.length() + 1];
	for (int i = 0; i < channelName.length(); i++) {
		name[i] = channelName[i] == '/' ? '-' : channelName[i];
	}
	name[channelName.length()] = '\0';
	std::string strName = std::string(name);
	
	m_mutex.lock();
	/*std::map<std::string, std::list<IckCore::SubscriberInfo> >::iterator it;
	if ((it = m_subscribers.find(strName)) != m_subscribers.end())
	{
		std::list<IckCore::SubscriberInfo>::iterator subscriberInfo;
		for (subscriberInfo = it->second.begin(); subscriberInfo != it->second.end(); ++subscriberInfo)
		{
			try
			{
				std::string endpoint = subscriberInfo->objectId + ":" + subscriberInfo->endPoint;
				Ice::ObjectPrx prx = m_topic->getPublisher()->ice_getCommunicator()->stringToProxy(endpoint);
				IckCore::IckSubscriberPrx subscriberPrx = IckCore::IckSubscriberPrx::uncheckedCast(prx);
				subscriberPrx->update(strName, valueInfoPtr);
			}
			catch (const Ice::Exception& e) {
				std::cerr << e << std::endl;
				m_mutex.unlock();
				delete name;
				this->removeSubscriber(*subscriberInfo);
				return;
			}
			catch (const char* msg) {
				std::cerr << msg << std::endl;
				m_mutex.unlock();
				this->removeSubscriber(*subscriberInfo);
				delete name;
				return;
			}
		}	
	}*/

	delete name;
	m_mutex.unlock();
}

void IckStormPublisher::addSubscriber(const IckCore::SubscriberInfo& subscriberInfo/*, const ::Ice::Current&*/)
{
	m_mutex.lock();
	std::map<std::string, std::list<IckCore::SubscriberInfo> >::iterator it;
	if ((it = m_subscribers.find(subscriberInfo.objectId)) != m_subscribers.end())
	{
		it->second.push_back(subscriberInfo);
	}
	else
	{
		std::list<IckCore::SubscriberInfo> subscriberInfoList;
		subscriberInfoList.push_back(subscriberInfo);
		m_subscribers.insert(std::pair<std::string, std::list<IckCore::SubscriberInfo> >(subscriberInfo.objectId, subscriberInfoList));
	}

	// Ìí¼Ó¶©ÔÄ
	char* name = new char[subscriberInfo.objectId.length() + 1];
	for (int i = 0; i < subscriberInfo.objectId.length(); i++) {
		name[i] = subscriberInfo.objectId[i] == '-' ? '/' : subscriberInfo.objectId[i];
	}
	name[subscriberInfo.objectId.length()] = '\0';
	std::string strName = std::string(name);

	UntypedData* untypedData = dynamic_cast<UntypedData*>(
		ObjectManager::getInstance()->getObject(strName));
	if (untypedData != NULL)
	{
		untypedData->subscribe(this);
	}
	else
	{
		throw "subscribe failed exception, for channel " + strName;
	}

	m_mutex.unlock();
}

void IckStormPublisher::removeSubscriber(const IckCore::SubscriberInfo& subscriberInfo/*, const ::Ice::Current&*/)
{
	std::string channelName = subscriberInfo.objectId;
	std::string::size_type pos = 0;
	char* name = new char[channelName.length() + 1];
	for (int i = 0; i < channelName.length(); i++) {
		name[i] = channelName[i] == '/' ? '-' : channelName[i];
	}
	name[channelName.length()] = '\0';
	std::string strName = std::string(name);

	m_mutex.lock();
	std::map<std::string, std::list<IckCore::SubscriberInfo> >::iterator it;
	if ((it = m_subscribers.find(strName)) != m_subscribers.end())
	{
		std::list<IckCore::SubscriberInfo>::iterator endpoint;
		for (endpoint = it->second.begin(); endpoint != it->second.end(); ++it)
		{
			if (endpoint->endPoint.compare(subscriberInfo.endPoint) == 0)
			{
				it->second.erase(endpoint);
				break;
			}
		}
		if (it->second.size() == 0)
		{
			m_subscribers.erase(strName);
		}
	}
	m_mutex.unlock();
}

void IckStormPublisher::update(UntypedData* untypedData)
{
	if (!untypedData->isChanged()) return;

	IckCore::RemoteValueInfo valueInfo;
	valueInfo.timeStamp = time(0);
	
	ReadOnlyInt* iData = dynamic_cast<ReadOnlyInt*>(untypedData);
	if (iData != NULL)
	{
		valueInfo.accessMode = IckCore::READONLY;
		valueInfo.dataType = IckCore::INTDATA;
		// valueInfo.iValue = iData->getValue();
		valueInfo.iValue = iData->m_value;
		valueInfo.descriptor = "UnInit";

		this->publish(untypedData->getFullName(), valueInfo);
		return;
	}

	ReadOnlyDouble* dData = dynamic_cast<ReadOnlyDouble*>(untypedData);
	if (dData != NULL)
	{
		valueInfo.accessMode = IckCore::READONLY;
		valueInfo.dataType = IckCore::DOUBLEDATA;
		valueInfo.dValue = dData->m_value;

		this->publish(untypedData->getFullName(), valueInfo);
		return;
	}
}
