#include "DotNetInterface.h"
#include "XmlParser.h"


void IckCoreInit(bool isServer, bool isLoadConfig)
{
	app = new IApplication(isServer, isLoadConfig);
	
}

void IckCoreStartUp(bool isServer)
{
	const char* m[] = { 0 };
	app->main(0, m);
	if (!isServer && app->isNetworkLan())
	{
		//while (IckStormSubscriberManager::getInstance()->getTopicManager() == NULL)
		//{
		//	// std::cout << "IckCoreInit: IckStormSubscriberManager getTopicManager == NULL" << std::endl;
		//}
	}
}

void IckCoreShutDown()
{
	if (app != NULL)
	{
		delete app;
	}
}

void RegisterObject(const char* className, const char* fullName)
{
	std::string strClassName = className;
	std::string strFullName = fullName;
	ObjectManager::getInstance()->registerObject(strClassName, strFullName);
}

void Subscribe(const char* fullName, Subscriber* subscriber)
{
	std::cout << "Subscriber start: fullName: " << std::string(fullName)<<", Subscriber: "<<(unsigned int)subscriber << std::endl;
	// subscriber->update(NULL);
	
	IckStormSubscriberManager::getInstance()->subscriber(new IckStormSubscriber(std::string(fullName)), subscriber);
}

void SubscribeWithCallBack(const char* fullName, SubscirberCallBack callback)
{
	std::cout << "Subscriber start: fullName: " 
		<< std::string(fullName) << ", Callback: " 
		<< (unsigned int)callback << std::endl;
	// 如果不是局域网，则用Socket进行订阅(ICE不支持广域网，至少目前没有解决方法)
	std::cout << "----DEBUG: network lan: " << 
		(app->isNetworkLan()?std::string("LAN"):std::string("WAN")) << std::endl;
	if (!app->isNetworkLan())  
	{
		IckStormSubscriberManager::getInstance()->subscriber(std::string(fullName), callback);
	}
	else
	{
		IckStormSubscriberManager::getInstance()->subscriber(new IckStormSubscriber(std::string(fullName)), callback);
	}
}

void RunService(const char* objectName, const char* serviceName, const char* params)
{
	std::cout << "RunService start: objectName: "
		<< std::string(objectName) << ", serviceName: "
		<< std::string(serviceName) << ", params: "
		<< std::string(params) << std::endl;
	if (app != NULL)
	{
		app->runService(std::string(objectName), std::string(serviceName), params == NULL ? "" : std::string(params));
	}
}

void Unsubscribe(const char* fullName)
{
	IckStormSubscriberManager::getInstance()->unsubscriber(std::string(fullName));
}

void AddNodeInfoCallBack(NodeInfoCallBack callback)
{
	if (!app->isNetworkLan())
	{
		ObjectManager::getInstance()->addNodeInfoCallBack(callback);
	}
	else
	{
		ObjectManager::getInstance()->addNodeInfoCallBack(callback);
	}
}

void GetNodeInfoList(const char* nodeName)
{
	IckStormSubscriberManager::getInstance()->getNodeInfoList(std::string(nodeName));
}

void setNetworkType(bool isNetworkLan)
{
	if (app != NULL)
	{
		app->setNetworkType(isNetworkLan);
	}
}

void setIckServiceEndpoint(const char* endpoint)
{
	if (app != NULL)
	{
		app->setServiceEndPoint(std::string(endpoint));
	}
}

void setIckSubscriberEndpoint(const char* endpoint)
{
	if (app != NULL)
	{
		app->setSubscriberEndpoint(std::string(endpoint));
	}
}

void setIckTopicManagerEndpoint(const char* endpoint)
{
	if (app != NULL)
	{
		app->setTopicManagerEndPoint(std::string(endpoint));
	}
}

void setIckSubscriberListener(const char* host, int port)
{
	if (app != NULL)
	{
		app->setSubscriberListener(std::string(host), port);
	}
}

void setIckServiceListener(const char* host, int port)
{
	if (app != NULL)
	{
		app->setServiceListener(std::string(host), port);
	}
}

