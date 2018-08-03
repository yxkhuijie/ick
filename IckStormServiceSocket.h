#ifndef _ICK_STORM_SERVICE_SOCKET_H_
#define _ICK_STORM_SERVICE_SOCKET_H_

#include "ISocket.h"
#include <vector>
#include "IMutex.h"


class IckStormServiceSocket : public ISocket
{

public:

	IckStormServiceSocket();

	virtual ~IckStormServiceSocket();

public:
	struct ServiceParam
	{
	public:
		std::string objectName;
		std::string serviceName;
		std::string param;
		long long int  registTime;
	};

private:
	std::vector<ServiceParam> m_serviceToRun;

public:

	void setHost(const std::string& host);

	void setPort(int port);

	void setServer(bool isServer);

public:

	virtual bool socketConnectHandler(SocketParam* socketParam);

public:

	void addService(const std::string& objectName, const std::string& serviceName, const std::string& param);

	std::string encodeData(std::string data);

	std::string decodeData(std::string data);
};

#endif

