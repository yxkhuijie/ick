#ifndef _ICK_STORM_DATA_FORWARDER_H_
#define _ICK_STORM_DATA_FORWARDER_H_

#include "ISocket.h"
#include "ControlObject.h"
#include "Subscriber.h"

class ick_api IckStormDataForwarder : public ISocket, public ControlObject, public Subscriber
{
	DECLARE_CLASS_DYNAMIC(IckStormDataForwarder)
	DECLARE_MESSAGE_LIST
	DECLARE_SERVICEINFO_LIST

public:
	
	IckStormDataForwarder();
	
	~IckStormDataForwarder();

private:

	SOCKET m_socketClient;
	unsigned char m_buffRecvTmp[4096];
	std::list<std::string> m_messages;
	long long m_latestHeartBeatTime;

public:

	virtual void make();
	virtual void startup();
	virtual bool socketConnectHandler(SocketParam* socketParam);
	virtual void update(UntypedData* untypedData);

	void setHost(const std::string& host);
	void setPort(int port);
	void setServer(bool isServer);
	void setInterval(int interval);
};

#endif

