#ifndef _ICK_STORM_SUBSCRIBER_SOCKET_H_
#define _ICK_STORM_SUBSCRIBER_SOCKET_H_

#include "ISocket.h"
#include <vector>
#include "IckStormSubscriberManager.h"
#include "IMutex.h"

class IckStormSubscriberSocket : public ISocket, public Subscriber
{

public:
	
	IckStormSubscriberSocket();
	
	virtual ~IckStormSubscriberSocket();

public:

	void setHost(const std::string& host);

	void setPort(int port);
	
	void setServer(bool isServer);

	// 仅客户端使用
	std::map<std::string, ChannelSubscribeInfo> m_subscribeChannels;

	std::list<std::string> m_dataSendList;

    // std::map<SOCKET, std::map<std::string, ChannelSubscribe> > m_subscribeChannels;

public:
	
	virtual bool socketConnectHandler(SocketParam* socketParam);

	void socketClosedHandler(SocketParam* socketParam);

	bool subscribeChannelHandler(SocketParam* socketParam);

	// client use this function to subscribe data
	void subscribe(std::string channelName);

	void addSubscribe(SocketParam* socketParam, std::string channelName);

	void addSendData(std::string data);

	virtual void update(UntypedData* untypedData);

	int publish(SocketParam* socketParam, const std::string& channelName, const ::IckCore::RemoteValueInfo& valueInfo);

	std::string encodeData(std::string data);

	std::string decodeData(std::string data);
};

#endif
