#ifndef _DOT_NET_INTERFACE_H_
#define _DOT_NET_INTERFACE_H_


#include "IApplication.h"
#include "ObjectManager.h"
#include "IckStormSubscriberManager.h"
#include "Subscriber.h"

extern "C"
{
	// typedef void (*SubscribeCallBack)();

	// void IckCoreStartUp(int, char*[]);
	IApplication *app = NULL;

	ick_api void IckCoreInit(bool isServer = true, bool isLoadConfig = true);

	ick_api void IckCoreStartUp(bool isServer = true);

	ick_api void IckCoreShutDown();

	ick_api void RegisterObject(const char* className, const char* fullName);

	ick_api void Subscribe(const char* fullName, Subscriber* subscriber);

	ick_api void SubscribeWithCallBack(const char* fullName, SubscirberCallBack callback);

	ick_api void Unsubscribe(const char* fullName);

	ick_api void AddNodeInfoCallBack(NodeInfoCallBack callback);

	ick_api void GetNodeInfoList(const char* nodeName);

	ick_api void RunService(const char* objectName, const char* serviceName, const char* params = NULL);

	ick_api void setNetworkType(bool isNetworkLan);

	ick_api void setIckServiceEndpoint(const char* endpoint);

	ick_api void setIckSubscriberEndpoint(const char* endpoint);

	ick_api void setIckTopicManagerEndpoint(const char* endpoint);

	ick_api void setIckSubscriberListener(const char* host, int port);

	ick_api void setIckServiceListener(const char* host, int port);
};

#endif
