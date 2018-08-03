
#include "IckStormManager.h"
#include "IckStormSubscriberSocket.h"
#include "IckStormServiceSocket.h"

IckStormManager* IckStormManager::m_instance = NULL;

IckStormManager::IckStormManager()
{
	this->m_subscriberSocket = NULL;
	this->m_serviceSocket = NULL;
}


IckStormManager::~IckStormManager()
{
	/*m_communicator->destroy();*/
}

IckStormManager* IckStormManager::getInstance()
{
	if (m_instance == NULL)
	{
		if (m_instance == NULL)
		{
			m_instance = new IckStormManager();
		}
	}

	return m_instance;
}
//
//void IckStormManager::setSubscriberAdapter(Ice::ObjectAdapterPtr adapter)
//{
//	this->m_subscriberAdapter = adapter;
//}
//
//void IckStormManager::setTopicManager(IceStorm::TopicManagerPrx topicManager)
//{
//	this->m_topicManager = topicManager;
//}
//
//IceStorm::TopicManagerPrx IckStormManager::getTopicManager()
//{
//	return this->m_topicManager;
//}

bool IckStormManager::subscriber(const std::string& channelName, IckStormSubscriber* subscirber, SubscirberCallBack callback)
{
	return false;
}

bool IckStormManager::subscriber(IckStormSubscriber* subscirber, SubscirberCallBack callback)
{
	m_mutex.lock();
	if (subscirber->getChannelNameAsIdentity().compare("") == 0)
	{
		throw "IckStormManager::subscriber: subscriber identity is empty";
	}
	std::cout << "Subscriber start: fullName: " << subscirber->getChannelNameAsIdentity() << std::endl;
	/*IckCore::IckSubscriberPtr subscriber = subscirber;*/
	try
	{
		std::cout << "----DEBUG: " << "subscriber channel name: ----  " << subscirber->getChannelNameAsIdentity() << std::endl;
		//std::cout << "----DEBUG: " << "subscriber adapter: " << (unsigned int)m_subscriberAdapter._ptr <<std::endl;
		
		/*Ice::ObjectPrx subscriberPrx = m_subscriberAdapter->add(
			subscriber, Ice::stringToIdentity(subscirber->getChannelNameAsIdentity()));
		std::cout << "----DEBUG: " << "++++++++++++++ " << std::endl;
		IceStorm::TopicPrx topic;
		try {
			std::cout << "----DEBUG: " << "-------------- " << std::endl;
			topic = m_topicManager->retrieve("IO");
			std::cout << "----DEBUG: " << "++++++++++++++-------------- " << std::endl;
			IceStorm::QoS qos;

			topic->subscribeAndGetPublisher(qos, subscriberPrx);
			std::cout << "----DEBUG: " << "--------------++++++++++++++ " << std::endl;
		}
		catch (const IceStorm::NoSuchTopic&) {
			std::cout << "Subscribe failure, no such topic found!" << std::endl;
		}*/

		if (callback != NULL)
		{
			std::map<std::string, std::list<SubscirberCallBack> >::iterator it;
			if ((it = this->m_callbacks.find(subscirber->getChannelNameAsIdentity())) != this->m_callbacks.end())
			{
				it->second.push_back(callback);
			}
			else
			{
				std::list<SubscirberCallBack> callbackList;
				callbackList.push_back(callback);
				this->m_callbacks.insert(std::pair<std::string, std::list<SubscirberCallBack> >(subscirber->getChannelNameAsIdentity(), callbackList));
			}
		}
	}
	/*catch (const Ice::Exception& e) {
		std::cout << e.what() << std::endl;
	}*/
	catch (const char* msg) {
		std::cout << msg << std::endl;
	}
	m_mutex.unlock();
}

bool IckStormManager::subscriber(IckStormSubscriber* remoteSubscirber, Subscriber* subscriber)
{
	/*m_mutex.lock();
	if (remoteSubscirber->getChannelNameAsIdentity().compare("") == 0)
	{
		throw "IckStormManager::subscriber: subscriber identity is empty";
	}
	std::cout << "Subscriber start: fullName: " << remoteSubscirber->getChannelNameAsIdentity() << std::endl;
	IckCore::IckSubscriberPtr subscriberPtr = remoteSubscirber;
	try
	{
		Ice::ObjectPrx subscriberPrx = m_subscriberAdapter->add(
			subscriberPtr, Ice::stringToIdentity(remoteSubscirber->getChannelNameAsIdentity()));
		std::cout << "----DEBUG: " << "subscriber channel name: "  << std::endl;

		IceStorm::TopicPrx topic;
		try {
			topic = m_topicManager->retrieve("IO");
			IceStorm::QoS qos;
			topic->subscribeAndGetPublisher(qos, subscriberPrx);
		}
		catch (const IceStorm::NoSuchTopic&) {
			m_mutex.unlock();
			return false;
		}

		if (subscriber != NULL)
		{
			std::map<std::string, std::list<Subscriber*> >::iterator it;
			if ((it=this->m_subscriberCallBack.find(remoteSubscirber->getChannelNameAsIdentity())) != this->m_subscriberCallBack.end())
			{
				it->second.push_back(subscriber);
			}
			else
			{
				std::list<Subscriber*> subscriberList;
				subscriberList.push_back(subscriber);
				this->m_subscriberCallBack.insert(std::pair<std::string, std::list<Subscriber*> >(remoteSubscirber->getChannelNameAsIdentity(), subscriberList));
			}
		}

		std::cout << "Subscriber finish: fullName: " << remoteSubscirber->getChannelNameAsIdentity()<< std::endl;
	}
	catch (const Ice::Exception& e) {
		std::cout << "Data " << remoteSubscirber->getChannelName() << " subscribe failed! Exception occured in IckStormManager::subscriber!";
		std::cerr << e << std::endl;
	}
	catch (const char* msg) {
		std::cout << "Data " << remoteSubscirber->getChannelName() << " subscribe failed! Exception occured in IckStormManager::subscriber! msg: ";
		std::cerr << msg << std::endl;
	}
	m_mutex.unlock();*/
	return true;
}

bool IckStormManager::subscriber(const std::string& channelName, SubscirberCallBack callback)
{
	std::cout << "Subscriber start: fullName: " << channelName << std::endl;
	// return this->subscriber(new IckStormSubscriber(channelName), callback);

	char* name = new char[channelName.length() + 1];
	for (int i = 0; i < channelName.length(); i++) {
		name[i] = channelName[i] == '/' ? '-' : channelName[i];
	}
	name[channelName.length()] = '\0';
	std::string channelPath = std::string(name);

	m_mutex.lock();
	// 向远程Socket发送订阅消息
	if (this->m_subscriberSocket == NULL)
	{
		std::cout << "subscirbe failure, subscribe socket is null!" << std::endl;
		return false;
	}
	this->m_subscriberSocket->subscribe(channelName);

	if (callback != NULL)
	{
		std::map<std::string, std::list<SubscirberCallBack> >::iterator it;
		if ((it = this->m_callbacks.find(channelPath)) != this->m_callbacks.end())
		{
			it->second.push_back(callback);
		}
		else
		{
			std::list<SubscirberCallBack> callbackList;
			callbackList.push_back(callback);
			this->m_callbacks.insert(std::pair<std::string, std::list<SubscirberCallBack> >(channelPath, callbackList));
		}
	}
	m_mutex.unlock();
	delete[] name;

}

bool IckStormManager::subscriber(const std::string& channelName, Subscriber* subscriber)
{
	std::cout << "Subscriber start: fullName: " << channelName << std::endl;
	// return this->subscriber(new IckStormSubscriber(channelName), callback);

	char* name = new char[channelName.length() + 1];
	for (int i = 0; i < channelName.length(); i++) {
		name[i] = channelName[i] == '/' ? '-' : channelName[i];
	}
	name[channelName.length()] = '\0';
	std::string channelPath = std::string(name);

	m_mutex.lock();
	// 向远程Socket发送订阅消息
	if (this->m_subscriberSocket == NULL)
	{
		std::cout << "subscirbe failure, subscribe socket is null!" << std::endl;
		m_mutex.unlock();
		return false;
	}
	this->m_subscriberSocket->subscribe(channelName);

	if (subscriber != NULL)
	{
		std::map<std::string, std::list<Subscriber*> >::iterator it;
		if ((it = this->m_subscriberCallBack.find(channelPath)) != this->m_subscriberCallBack.end())
		{
			it->second.push_back(subscriber);
		}
		else
		{
			std::list<Subscriber*> subscriberList;
			subscriberList.push_back(subscriber);
			this->m_subscriberCallBack.insert(std::pair<std::string, std::list<Subscriber*> >(channelPath, subscriberList));
		}
	}
	m_mutex.unlock();
	delete[] name;
	return true;
}

bool IckStormManager::unsubscriber(const std::string& channelName)
{
	// IckCore::IckSubscriberPtr subscriberPrx = m_subscriberAdapter->find(Ice::stringToIdentity(channelName));
	/*IckCore::IckSubscriberPrx subscriberPrx = IckCore::IckSubscriberPrx::checkedCast(m_subscriberAdapter->getCommunicator()->stringToProxy(channelName));

	IceStorm::TopicPrx topic;
	try {
		topic = m_topicManager->retrieve("IO");
		topic->unsubscribe(subscriberPrx);
	}
	catch (const IceStorm::NoSuchTopic&) {
		return false;
	}*/
	return true;
}

std::list<Subscriber*> IckStormManager::getSubscriberList(const std::string& channelName)
{
	char* name = new char[channelName.length() + 1];
	for (int i = 0; i < channelName.length(); i++) {
		name[i] = channelName[i] == '/' ? '-' : channelName[i];
	}
	name[channelName.length()] = '\0';
	std::string channelPath = std::string(name);

	std::map<std::string, std::list<Subscriber*> >::iterator it;
	if ((it=this->m_subscriberCallBack.find(channelPath)) != this->m_subscriberCallBack.end())
	{
		return it->second;
	}
	else
	{
		std::list<Subscriber*> subscriberList;
		return subscriberList;
	}
}

std::list<SubscirberCallBack> IckStormManager::getSubscriberCallBackList(const std::string& channelName)
{
	char* name = new char[channelName.length() + 1];
	for (int i = 0; i < channelName.length(); i++) {
		name[i] = channelName[i] == '/' ? '-' : channelName[i];
	}
	name[channelName.length()] = '\0';
	std::string channelPath = std::string(name);
	
	

	std::map<std::string, std::list<SubscirberCallBack > >::iterator it;
	if ((it = this->m_callbacks.find(channelPath)) != this->m_callbacks.end())
	{
		delete[]  name;
		return it->second;
	}
	else
	{
		int nodeIndex = channelName.find_last_of("/");
		std::string nodeName = channelName.substr(0, nodeIndex + 1);
		for (int i = 0; i < nodeName.length(); i++)
		{
			if (nodeName.at(i) == '/') nodeName[i] = '-';
		}

		if ((it = this->m_callbacks.find(nodeName)) != this->m_callbacks.end())
		{
			delete[]  name;
			return it->second;
		}
		else
		{
			delete[]  name;
			std::list<SubscirberCallBack> callbackList;
			return callbackList;
		}
	}
	
}

void IckStormManager::setIckStormSubscriberSocket(IckStormSubscriberSocket* socket)
{
	if (socket != NULL)
	{
		this->m_subscriberSocket = socket;
	}
}

void IckStormManager::setIckStormServiceSocket(IckStormServiceSocket* socket)
{
	if (socket != NULL)
	{
		this->m_serviceSocket= socket;
	}
}

void IckStormManager::getNodeInfoList(std::string& nodeName)
{
	if (nodeName.empty()) return;
	if (this->m_subscriberSocket != NULL)
	{
		std::string data = "ChannelList:" + nodeName + ";";
		m_subscriberSocket->addSendData(data);
	}
}

void IckStormManager::runService(const std::string& objectName, const std::string& serviceName, const std::string& param)
{
	if(this->m_serviceSocket != NULL)
	{
		this->m_serviceSocket->addService(objectName, serviceName, param);
	}
}

void IckStormManager::update(UntypedData* data)
{
	if (!data->isChanged()) return;

	std::string fullName = data->getFullName();
}

