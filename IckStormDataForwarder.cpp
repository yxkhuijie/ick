#include "IckStormDataForwarder.h"
#include "ObjectManager.h"
#include "ReadOnlyInt.h"
#include "ReadOnlyDouble.h"
#include "ReadOnlyString.h"
#include "ReadWriteInt.h"
#include "ReadWriteDouble.h"
#include "ReadWriteString.h"
#include "Converter.h"
#include "TimeConverter.h"
#ifdef __linux__
#include <string.h>
#endif

IckStormDataForwarder::IckStormDataForwarder()
{
	this->m_isClient = true;
	this->m_latestHeartBeatTime = 0;
}


IckStormDataForwarder::~IckStormDataForwarder()
{
}

void IckStormDataForwarder::make()
{

}

void IckStormDataForwarder::startup()
{
	std::map<std::string, UntypedData*>* datas = ObjectManager::getInstance()->getDatas();
	if (datas != NULL)
	{
		std::map<std::string, UntypedData*>::iterator it;
		for (it = datas->begin(); it != datas->end(); ++it)
		{
			if (it->second != NULL)
			{
				it->second->subscribe(this);
			}
		}
	}
	this->connect();
}

bool IckStormDataForwarder::socketConnectHandler(SocketParam* socketParam)
{
	ControlObject::msleep(200);
	if (socketParam->socketHandle != m_socketClient) m_socketClient = socketParam->socketHandle;
	// unsigned char buff[4096];
	memset(m_buffRecvTmp, 0, sizeof(m_buffRecvTmp));
	int len = 0;
	std::string strTmp = "";

	this->m_mutex.lock();
	std::string str = "";
	std::list<std::string>::iterator it;
	int index = 0;
	for (it = this->m_messages.begin(); it != this->m_messages.end(); ++it)
	{
		Logger::getInstance()->Debug(*it);
		str += *it;
		if (index++ >= 10)
		{
			len = ::send(this->m_socketClient, str.c_str(), str.length(), 0);
			index = 0;
			str = "";

			if (len == -1)
			{
#ifdef __windows__
				int id = WSAGetLastError();
				if (id == WSAECONNRESET || id == WSAECONNABORTED)
				{
					std::cout << "socket error: id: " << id << " position: 3" << std::endl;
					return false;
				}
				else if (id != WSAETIMEDOUT)
				{
					std::cout << "socket error: id: " << id << std::endl;
				}
#elif __linux__
				return false;
#endif
			}
			else if (len == 0)
			{
				std::cout << "socket error: len: 0" << std::endl;
				return false;
			}
		}
	}
	this->m_messages.clear();
	this->m_mutex.unlock();

	if (str.compare("") != 0)
	{
		len = ::send(this->m_socketClient, str.c_str(), str.length(), 0);
		if (len == -1)
		{
#ifdef __windows__
			int id = WSAGetLastError();
			if (id == WSAECONNRESET || id == WSAECONNABORTED)
			{
				std::cout << "socket error: id: " << id << " position: 3" << std::endl;
				return false;
			}
			else if (id != WSAETIMEDOUT)
			{
				std::cout << "socket error: id: " << id << std::endl;
			}
#elif __linux__
			return false;
#endif
		}
		else if (len == 0)
		{
			std::cout << "socket error: len: 0" << std::endl;
			return false;
		}
	}

	// ÐÄÌø
	long currentTime = TimeConverter::getCurrentTimeAsSeconds();
	if (currentTime - this->m_lastHeartBeatTime > 60)
	{
		str = "HeartBeat;";
		len = ::send(this->m_socketClient, str.c_str(), str.length(), 0);
		this->m_lastHeartBeatTime = currentTime;

		if (len == -1)
		{
#ifdef __windows__
			int id = WSAGetLastError();
			if (id == WSAECONNRESET || id == WSAECONNABORTED)
			{
				std::cout << "socket error: id: " << id << " position: 3" << std::endl;
				return false;
			}
			else if (id != WSAETIMEDOUT)
			{
				std::cout << "socket error: id: " << id << std::endl;
			}
#elif __linux__
			return false;
#endif
		}
		else if (len == 0)
		{
			std::cout << "socket error: len: 0" << std::endl;
			return false;
		}
	}
	return true;
}

void IckStormDataForwarder::update(UntypedData* untypedData)
{
	if (untypedData == NULL) return;
	std::string channelName = untypedData->getFullName();
	std::string str = "Update:" + channelName;

	IckCore::RemoteValueInfo valueInfo;
	valueInfo.timeStamp = time(0);
	valueInfo.dValue = 0;
	valueInfo.iValue = 0;
	valueInfo.sValue = "";

	ReadOnlyInt* iData = dynamic_cast<ReadOnlyInt*>(untypedData);
	ReadOnlyDouble* dData = dynamic_cast<ReadOnlyDouble*>(untypedData);
	ReadWriteInt* wiData = dynamic_cast<ReadWriteInt*>(untypedData);
	ReadWriteDouble* wdData = dynamic_cast<ReadWriteDouble*>(untypedData);
	ReadOnlyString* sData = dynamic_cast<ReadOnlyString*>(untypedData);
	ReadWriteString* wsData = dynamic_cast<ReadWriteString*>(untypedData);
	if (iData != NULL)
	{
		valueInfo.accessMode = IckCore::READONLY;
		valueInfo.dataType = IckCore::INTDATA;
		valueInfo.iValue = iData->getValue();
		valueInfo.descriptor = "UnInit";
	}
	else if (dData != NULL)
	{
		valueInfo.accessMode = IckCore::READONLY;
		valueInfo.dataType = IckCore::DOUBLEDATA;
		valueInfo.dValue = dData->getValue();
	}
	else if (wiData != NULL)
	{
		valueInfo.accessMode = IckCore::READWRITE;
		valueInfo.dataType = IckCore::INTDATA;
		valueInfo.iValue = wiData->getValue();
	}
	else if (wdData != NULL)
	{
		valueInfo.accessMode = IckCore::READWRITE;
		valueInfo.dataType = IckCore::DOUBLEDATA;
		valueInfo.dValue = wdData->getValue();
	}
	else if (sData != NULL)
	{
		valueInfo.accessMode = IckCore::READONLY;
		valueInfo.dataType = IckCore::STRINGDATA;
		valueInfo.sValue = sData->getValue();
	}
	else if (wsData != NULL)
	{
		valueInfo.accessMode = IckCore::READWRITE;
		valueInfo.dataType = IckCore::STRINGDATA;
		valueInfo.sValue = wsData->getValue();
	}


	if (valueInfo.accessMode == IckCore::DEFAULT) str += std::string(":U");
	else if (valueInfo.accessMode == IckCore::READONLY) str += std::string(":R");
	else if (valueInfo.accessMode == IckCore::READWRITE) str += std::string(":W");
	if (valueInfo.dataType == IckCore::UNKNOWN) str += std::string(":U");
	else if (valueInfo.dataType == IckCore::INTDATA) str += std::string(":I");
	else if (valueInfo.dataType == IckCore::STRINGDATA) str += std::string(":S");
	else if (valueInfo.dataType == IckCore::DOUBLEDATA) str += std::string(":D");

	if (valueInfo.dataType == IckCore::UNKNOWN) str += std::string(":");
	else if (valueInfo.dataType == IckCore::INTDATA) str += std::string(":") + Converter::convertToString(valueInfo.iValue);
	else if (valueInfo.dataType == IckCore::STRINGDATA) str += std::string(":") + (/*valueInfo.sValue.empty() ? "?????" : */valueInfo.sValue);
	else if (valueInfo.dataType == IckCore::DOUBLEDATA) str += std::string(":") + Converter::convertToString(valueInfo.dValue);

	str += std::string(":") + Converter::convertToString(time(0));
	str += ";";
	// std::cout << "update data: " << channelName << ": iValue: " << valueInfo.iValue << " dValue: " << valueInfo.dValue << std::endl;

	this->m_mutex.lock();
	m_messages.push_back(str);
	this->m_mutex.unlock();

}

void IckStormDataForwarder::setHost(const std::string& host)
{
	this->m_host = host;
}

void IckStormDataForwarder::setPort(int port)
{
	this->m_port = port;
}

void IckStormDataForwarder::setServer(bool isServer)
{
	this->m_isClient = !isServer;
}

void IckStormDataForwarder::setInterval(int interval)
{
	this->m_interval = interval;
}



IMPLEMENT_CLASS_DYNAMIC(IckStormDataForwarder, ControlObject)

BEGIN_SERVICEINFO_LIST(IckStormDataForwarder)
END_SERVICEINFO_LIST(IckStormDataForwarder)

IMPLEMENT_MESSAGE_LIST(IckStormDataForwarder, ControlObject)

BEGIN_MESSAGE_LIST(IckStormDataForwarder)
	SET_S(setHost, IckStormDataForwarder)
	SET_I(setPort, IckStormDataForwarder)
	SET_B(setServer, IckStormDataForwarder)
	SET_I(setInterval, IckStormDataForwarder)
END_MESSAGE_LIST
