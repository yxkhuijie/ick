#include "IckStormServiceSocket.h"
#include "Converter.h"
#include "ReadOnlyDouble.h"
#include "ReadOnlyInt.h"
#include "ControlObject.h"
#include "ObjectManager.h"

#ifdef __linux__
#include <string.h>
#include <arpa/inet.h>
#endif

IckStormServiceSocket::IckStormServiceSocket()
{
	this->m_isActiveMode = false;
	this->m_interval = 200;
	this->m_timeout = 1000;
}


IckStormServiceSocket::~IckStormServiceSocket()
{

}

bool IckStormServiceSocket::socketConnectHandler(SocketParam* socketParam)
{
	if (m_isClient)
	{
		int flag = 0;
		m_mutex.lock();
		if (this->m_serviceToRun.size() > 0)
		{
			std::vector<ServiceParam>::iterator it;
			for (it = m_serviceToRun.begin(); it != m_serviceToRun.end(); )
			{
				std::string str = "RunService:" + it->objectName + std::string(":") + it->serviceName + std::string(":") + it->param + std::string(";");
				if ((flag = this->send(str, socketParam)) < 0) return false;
				it = m_serviceToRun.erase(it);
			}
		}

		long long t = time(0);
		if (t - this->m_lastHeartBeatTime > 60)
		{
			std::string str = "HeartBeat;";
			if ((flag = this->send(str, socketParam)) < 0) return false;
			this->m_lastHeartBeatTime = t;
		}

		m_mutex.unlock();
	}
	else
	{
		char buff[4096];
		memset(buff, 0, sizeof(buff));
		int len = 0;
		std::string strTmp = "";
		int flag = 0;
		while ((flag = len = this->receive(buff + strTmp.length(), sizeof(buff) - 1 - strTmp.length(), socketParam)) > 0)
		{
			std::string data = "";
			data = std::string(buff);
			std::string message = std::string(inet_ntoa(socketParam->socketAddress.sin_addr)) + " : " +
				Converter::convertToString(socketParam->socketAddress.sin_port) + ": RX: " + data;
			Logger::getInstance()->Debug(message);

			std::vector<std::string> datas = Converter::split(data, ";");

			int dataSize = (len == sizeof(buff) - 1 - strTmp.length()) ? datas.size() - 1 : datas.size();
			for (int i = 0; i < dataSize; i++)
			{
				std::vector<std::string> tmp = Converter::split(datas[i], ":");
				int size = tmp.size();
				std::string sendData = "";

				if (size > 2 && tmp[0].compare("RunService") == 0)
				{
					std::string objectName = tmp[1];
					std::string serviceName = tmp[2];
					std::string params = size > 3 ? tmp[3] : "";

					sendData = "RunService:Success:" + objectName + ":" + serviceName + ";";
					if ((flag = this->send(sendData, socketParam)) < 0)
					{ 
						break; 
					}
					sendData = "";
					ControlObject* controlObject = dynamic_cast<ControlObject*>(ObjectManager::getInstance()->getObject(objectName));
					if (controlObject != NULL)
					{
						controlObject->call(serviceName, params);
					}
				}
				else if (size > 0 && tmp[0].compare("HeartBeat") == 0)
				{
					// heart beat;
				}
				else
				{
					std::string message = std::string(inet_ntoa(socketParam->socketAddress.sin_addr)) + " : " +
						Converter::convertToString(socketParam->socketAddress.sin_port) + ": Data format received error: data: " + data;
					Logger::getInstance()->Warn(message);
					std::cout << TimeConverter::getCurrentTimeAsStr() << ": " << message << std::endl;
				}

				if (!sendData.empty())
				{
					if ((flag = this->send(sendData, socketParam)) < 0) break;
				}
				if (len == sizeof(buff) - 1 - strTmp.length())
				{
					strTmp = datas[datas.size() - 1];
					memset(buff, 0, sizeof(buff));
					memcpy(buff, strTmp.c_str(), strTmp.length());
				}
				else
				{
					strTmp = "";
					memset(buff, 0, sizeof(buff));
				}
			}
		}
		if (flag < 0) return false;
	}
	if (this->m_interval > 0) this->msleep(m_interval);
	return true;
}

void IckStormServiceSocket::addService(const std::string& objectName,const std::string& serviceName, const std::string& param)
{
	m_mutex.lock();
	ServiceParam serviceParam;
	serviceParam.objectName = objectName;
	serviceParam.serviceName = serviceName;
	serviceParam.param = param;
	this->m_serviceToRun.push_back(serviceParam);
	m_mutex.unlock();
}

void IckStormServiceSocket::setHost(const std::string& host)
{
	this->m_host = host;
}

void IckStormServiceSocket::setPort(int port)
{
	this->m_port = port;
}
void IckStormServiceSocket::setServer(bool isServer)
{
	this->m_isClient = !isServer;
}

std::string IckStormServiceSocket::encodeData(std::string data)
{
	std::string res = "";
	for (int i = 0; i < data.length(); i++)
	{
		if (data.at(i) == ':')
		{
			res += "%3A";
		}
		else if (data.at(i) == ',')
		{
			res += "%2C";
		}
		else
		{
			res += data.at(i);
		}
	}
	return res;
}

std::string IckStormServiceSocket::decodeData(std::string data)
{
	std::string res = "";
	for (int i = 0; i < data.length(); i++)
	{
		if (i < data.length() - 2 &&
			data.at(i) == '%' && data.at(i + 1) == '3' && data.at(i + 2) == 'A')
		{
			res += ":";
			i += 2;
		}
		else if (i < data.length() - 2 &&
			data.at(i) == '%' && data.at(i + 1) == '2' && data.at(i + 2) == 'C')
		{
			res += ",";
			i += 2;
		}
		else
		{
			res += data.at(i);
		}
	}
	return res;
}
