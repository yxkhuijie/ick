#include "IckStormSubscriberSocket.h"
#include "Converter.h"
#include "ReadOnlyDouble.h"
#include "ReadOnlyInt.h"
#include "ReadWriteDouble.h"
#include "ReadWriteInt.h"
#include "ReadOnlyString.h"
#include "ReadWriteString.h"
#ifdef __linux__
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#endif

IckStormSubscriberSocket::IckStormSubscriberSocket()
{
	this->m_isActiveMode = false;
	this->m_interval     = 200;
	this->m_timeout      = 1000;
	this->m_recvBlockMode = false;
}


IckStormSubscriberSocket::~IckStormSubscriberSocket()
{

}

bool IckStormSubscriberSocket::socketConnectHandler(SocketParam* socketParam)
{
	SOCKET socket = socketParam->socketHandle;
	if (m_isClient)
	{
		try
		{
			if (this->subscribeChannelHandler(socketParam) == false)
			{
				return false;
			}

			char buff[4096];
			memset(buff, 0, sizeof(buff));
			int len = 0;
			std::string strTmp = "";
			// 循环接收数据
			while ((len = this->receive(buff + strTmp.length(), sizeof(buff) - 1 - strTmp.length(), socketParam)) > 0)
			{
				std::string s = std::string(buff);
				std::vector<std::string> datas = Converter::split(s, ";");
				int dataSize = (len == sizeof(buff) - 1 - strTmp.length()) ? datas.size() - 1 : datas.size();

				for (int i = 0; i < dataSize; i++)
				{
					std::vector<std::string> tmp = Converter::split(datas[i], ":");
					int size = tmp.size();
					if (size > 5 && tmp[0].compare("Update") == 0)
					{
						std::string channelName = tmp[1];
						std::string accessMode = tmp[2];
						std::string dataType = tmp[3];
						std::string sValue = this->decodeData(tmp[4]);
						std::string timeStamp = tmp[5];

						IckStormValueInfo valueInfo;
						memset(&valueInfo, 0, sizeof(valueInfo));
						if (accessMode.compare("U") == 0) valueInfo.accessMode = DEFAULT;
						else if (accessMode.compare("R") == 0) valueInfo.accessMode = READONLY;
						else if (accessMode.compare("W") == 0) valueInfo.accessMode = READWRITE;
						if (dataType.compare("0") == 0) valueInfo.dataType = UNKNOWN;
						else if (dataType.compare("I") == 0) valueInfo.dataType = INTDATA;
						else if (dataType.compare("S") == 0) valueInfo.dataType = STRINGDATA;
						else if (dataType.compare("D") == 0) valueInfo.dataType = DOUBLEDATA;

						char *endptr;
						valueInfo.iValue = atoi(sValue.c_str());
						valueInfo.dValue = atof(sValue.c_str());
						memcpy(valueInfo.sValue, sValue.c_str(), sValue.length());
						// valueInfo.timeStamp = strtoll(timeStamp.c_str(), &endptr, 10);
						valueInfo.timeStamp = atoi(timeStamp.c_str());

						UntypedData* untypedData = NULL;
						if (ObjectManager::getInstance()->isObjectRegisted(channelName) == false)
						{
							if (valueInfo.accessMode == READONLY && valueInfo.dataType == INTDATA)
							{
								untypedData = new ReadOnlyInt(valueInfo.iValue);
							}
							else if (valueInfo.accessMode == READONLY && valueInfo.dataType == DOUBLEDATA)
							{
								untypedData = new ReadOnlyDouble(valueInfo.dValue);
							}
							else if (valueInfo.accessMode == READONLY && valueInfo.dataType == STRINGDATA)
							{
								untypedData = new ReadOnlyString(sValue);
							}
							else if (valueInfo.accessMode == READWRITE && valueInfo.dataType == INTDATA)
							{
								untypedData = new ReadWriteInt(valueInfo.iValue);
							}
							else if (valueInfo.accessMode == READWRITE && valueInfo.dataType == DOUBLEDATA)
							{
								untypedData = new ReadWriteDouble(valueInfo.dValue);
							}
							else if (valueInfo.accessMode == READWRITE && valueInfo.dataType == STRINGDATA)
							{
								untypedData = new ReadWriteString(sValue);
							}

							if (untypedData != NULL)
							{
								ObjectManager::getInstance()->registerObject(untypedData, channelName);
							}
						}
						else
						{
							untypedData = dynamic_cast<UntypedData*>(ObjectManager::getInstance()->getObject(channelName));
							if (/*valueInfo.accessMode == READONLY && */valueInfo.dataType == INTDATA)
							{
								ReadWriteInt* data = dynamic_cast<ReadWriteInt*>(untypedData);
								data->setValue(valueInfo.iValue);
							}
							else if (/*valueInfo.accessMode == READONLY && */ valueInfo.dataType == DOUBLEDATA)
							{
								ReadWriteDouble* data = dynamic_cast<ReadWriteDouble*>(untypedData);
								data->setValue(valueInfo.dValue);
							}
							else if (/*valueInfo.accessMode == READONLY && */ valueInfo.dataType == STRINGDATA)
							{
								ReadWriteString* data = dynamic_cast<ReadWriteString*>(untypedData);
								data->setValue(sValue);
							}
							/*
							else if (valueInfo.accessMode == READWRITE && valueInfo.dataType == INTDATA)
							{
								ReadWriteInt* data = dynamic_cast<ReadWriteInt*>(untypedData);
								data->setValue(valueInfo.iValue);
							}
							else if (valueInfo.accessMode == READWRITE && valueInfo.dataType == DOUBLEDATA)
							{
								ReadWriteDouble* data = dynamic_cast<ReadWriteDouble*>(untypedData);
								data->setValue(valueInfo.dValue);
							}
							else if (valueInfo.accessMode == READWRITE && valueInfo.dataType == STRINGDATA)
							{
								ReadWriteString* data = dynamic_cast<ReadWriteString*>(untypedData);
								data->setValue(sValue);
							}
							*/
						}

						std::list<SubscirberCallBack> callbackList =
							IckStormSubscriberManager::getInstance()->getSubscriberCallBackList(channelName);
						if(callbackList.size() > 0)
						{
							std::list<SubscirberCallBack>::iterator it;
							for (it = callbackList.begin(); it != callbackList.end(); ++it)
							{
								(*it)(channelName.c_str(), &valueInfo);
							}
						}
						std::list<Subscriber*> subscriberList =
							IckStormSubscriberManager::getInstance()->getSubscriberList(channelName);
						if (subscriberList.size() > 0 && untypedData != NULL)
						{
							std::list<Subscriber*>::iterator it;
							for (it = subscriberList.begin(); it != subscriberList.end(); ++it)
							{
								(*it)->update(untypedData);
							}
						}
					}
					else if (size > 2 && tmp[0].compare("Subscribe") == 0 && tmp[1].compare("Success") == 0)
					{
						std::string channelName = tmp[2];
						m_mutex.lock();

						std::map<std::string, ChannelSubscribeInfo>::iterator it;
						if ((it = m_subscribeChannels.find(channelName)) != m_subscribeChannels.end())
						{
							it->second.isRegistSuccess = true;
							std::cout << "data " << channelName << " subscribed successful!" << std::endl;
						}

						m_mutex.unlock();
					}
					else if (size > 3 && tmp[0].compare("ChannelList") == 0)
					{
						std::string nodeName = tmp[1];
						int size = atoi(tmp[2].c_str());
						std::string nodeNames = tmp[3];
						std::vector<std::string> subNodes = Converter::split(nodeNames,",");
						std::list<NodeInfoCallBack> callbackList =
							ObjectManager::getInstance()->getNodeInfoCallBack();
						std::list<NodeInfoCallBack>::iterator it;
						for (it = callbackList.begin(); it != callbackList.end(); ++it)
						{
							if (*it == NULL) continue;
							IckStormNodeInfo nodeInfo;
							memset(&nodeInfo, 0, sizeof(nodeInfo));
							memcpy(nodeInfo.subNodeName, nodeNames.c_str(), nodeNames.length());
							nodeInfo.subNodeSize = subNodes.size();
							memcpy(nodeInfo.subNodeType, "Channel", 7);
							(*it)(nodeName.c_str(), &nodeInfo);
						}
					}
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

				this->subscribeChannelHandler(socketParam);
			}
			if (len < 0)
			{
				return false;
			}

			long long t = time(0);
			if (t - this->m_lastHeartBeatTime > 60)
			{
				std::string str = "HeartBeat;";
				int flag = 1;
				
				if (this->send(str, socketParam) < 0)
				{
					return false;
				}
				this->m_lastHeartBeatTime = t;
			}
		}
		catch (const char* err)
		{
			std::cout << "error message: " << err << std::endl;
		}
		catch (...)
		{
#ifdef __windows__
			int errId = ::GetLastError();
			std::cout << "error error error!!!: error id: "<< errId << std::endl;
#endif		
		}
	}
	else
	{
		char buff[4096];
		memset(buff, 0, sizeof(buff));
		int len = 0;
		int flag = 1;
		std::string strTmp = "";
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

				if (size > 0 && tmp[0].compare("HeartBeat") == 0)
				{
					// heart beat;
				}
				else if (size > 1 && tmp[0].compare("Subscribe") == 0)
				{
					std::string objectName = tmp[1];
					this->addSubscribe(socketParam, objectName);
					sendData = "Subscribe:Success:" + objectName + ";";
				}
				else if (size > 1 && tmp[0].compare("Unsubscribe") == 0)
				{
					std::string objectName = tmp[1];
					sendData = "Unsubscribe:Success:" + objectName + ";";
				}
				else if (size > 2 && tmp[0].compare("RunService") == 0)
				{
					std::string objectName = tmp[1];
					std::string serviceName = tmp[2];
					std::string params = size > 3 ? tmp[3] : "";

					sendData = "RunService:Success:" + objectName + ":" + serviceName + ";";
					if ((flag = ::send(socket, sendData.c_str(), sendData.length(), 0)) <= 0)
					{
						std::cout << "subscribe socket send failure: socket: " << socket << std::endl;
						break;
					}
					sendData = "";
					ControlObject* controlObject = dynamic_cast<ControlObject*>(ObjectManager::getInstance()->getObject(objectName));
					if (controlObject != NULL)
					{
						controlObject->call(serviceName, params);
					}
				}
				else if (size > 2 && tmp[0].compare("ChannelList") == 0)
				{
					std::string nodeName = tmp[1];
					std::list<std::string> subNodes = ObjectManager::getInstance()->getChannelNodeList(nodeName);
					int size = subNodes.size();
					sendData = "ChannelList:" + nodeName + ":" + Converter::convertToString(size) + ":";
					std::list<std::string>::iterator it;
					int index = 0;
					std::string strTmp = "";
					for (it=subNodes.begin(); it != subNodes.end(); ++it)
					{
						if (index < 10)
						{
							strTmp += *it + ((index == 9) ? ";" : ",");
							index++;
							continue;
						}
						else
						{
							sendData += Converter::convertToString(10) + ":" + strTmp;
							if (this->send(sendData, socketParam) < 0)
							{
								return false;
							}
							sendData = "ChannelList:" + nodeName + ":" + Converter::convertToString(size) + ":";
							strTmp = *it;
							index = 1;
						}
					}
					if (!strTmp.empty())
					{
						if (strTmp.at(strTmp.length() - 1) != ';') strTmp += ";";
						if (this->send(sendData, socketParam) < 0)
						{
							return false;
						}
					}
				}
				else if (size > 2 && tmp[0].compare("ServiceList") == 0)
				{
					std::string nodeName = tmp[1];
					std::list<std::string> subNodes = ObjectManager::getInstance()->getServiceNodeList(nodeName);
					int size = subNodes.size();
					sendData = "ServiceList:" + nodeName + ":" + Converter::convertToString(size) + ":";
					std::list<std::string>::iterator it;
					int index = 0;
					std::string strTmp = "";
					for (it = subNodes.begin(); it != subNodes.end(); ++it)
					{
						if (index < 10)
						{
							strTmp += *it + ((index == 9) ? ";" : ",");
							index++;
							continue;
						}
						else
						{
							sendData += Converter::convertToString(10) + ":" + strTmp;
							if (this->send(sendData, socketParam) < 0)
							{
								return false;
							}
							sendData = "ServiceList:" + nodeName + ":" + Converter::convertToString(size) + ":";
							strTmp = *it;
							index = 1;
						}
					}
					if (!strTmp.empty())
					{
						if (strTmp.at(strTmp.length() - 1) != ';') strTmp += ";";
						if (this->send(sendData, socketParam) < 0)
						{
							return false;
						}
					}
				}
				else
				{
					std::cout << "Data format received format error! data: " << datas[i] << std::endl << std::endl;
				}

				if (!sendData.empty())
				{
					if ((flag = this->send(sendData, socketParam)) < 0)
					{
						return false;
					}
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
		if (flag < 0)
		{
			return false;
		}
	}
	if (this->m_interval > 0) 
#ifdef __windows__
		msleep(m_interval);
#elif __linux__
		usleep(m_interval*1000);
#endif
	return true;
}

bool IckStormSubscriberSocket::subscribeChannelHandler(SocketParam* socketParam)
{
	m_mutex.lock();
	SOCKET socket = socketParam->socketHandle;
	SocketSubscribeChannelMap::iterator itSocketParam;

	int i = 0;
	std::string str = "";
	time_t t = time(0);

	std::map<std::string, ChannelSubscribeInfo>::iterator it;
	for (it = m_subscribeChannels.begin(); it != m_subscribeChannels.end(); ++it)
	{
		if (it->second.isRegistSuccess == false && (t - it->second.registTime > 5))
		{
			str += "Subscribe:" + it->first + std::string(";");
			it->second.registTime = t;
			if ((++i) % 10 == 0)
			{
				::send(socket, str.c_str(), str.length(), 0);
				msleep(100);
				str = "";
			}
		}
	}

	if (str != "") ::send(socket, str.c_str(), str.length(), 0);
	str = "";

	i = 0;
	std::list<std::string>::iterator it1;
	for (it1 = this->m_dataSendList.begin(); it1 != this->m_dataSendList.end(); ++it1)
	{
		str += *it1;
		if ((++i) % 10 == 0)
		{
			if(this->send(str, socketParam) < 0) return false;
			msleep(100);
			str = "";
		}
	}

	if (str != "") if (this->send(str, socketParam) < 0) return false;
	str = "";

	m_dataSendList.clear();
	m_mutex.unlock();
	return true;
	/*
	if ((itSocketParam = m_clientSocketMap.find(socketParam)) != m_clientSocketMap.end())
	{
		ChannelSubscribeInfoMap* channels = &itSocketParam->second;
		ChannelSubscribeInfoMap::iterator itChannelInfo;

		int i = 0;
		std::string str = "";
		time_t t = time(0);
		for (itChannelInfo = channels->begin(); itChannelInfo != channels->end(); ++itChannelInfo)
		{
			if (itChannelInfo->second.isRegistSuccess == false && (t - itChannelInfo->second.registTime > 5))
			{
				str += "Subscribe:" + itChannelInfo->first->getFullName() + std::string(";");
				itChannelInfo->second.registTime = t;
				if ((++i) % 10 == 0)
				{
					::send(socket, str.c_str(), str.length(), 0);
					msleep(200);
					str = "";
				}
			}
		}

		if (str != "") ::send(socket, str.c_str(), str.length(), 0);
		str = "";
	}
	*/
	
}

void IckStormSubscriberSocket::socketClosedHandler(SocketParam* socketParam)
{
	
}

// 此函数是客户端执行的
void IckStormSubscriberSocket::subscribe(std::string channelName)
{
	m_mutex.lock();
	ChannelSubscribeInfo channelSubscribe;
	channelSubscribe.channelName = channelName;
	channelSubscribe.registTime = 0;
	channelSubscribe.isRegistSuccess = false;

	std::map<std::string, ChannelSubscribeInfo>::iterator it;
	if ((it = m_subscribeChannels.find(channelName)) == m_subscribeChannels.end())
	{
		m_subscribeChannels.insert(std::pair<std::string, ChannelSubscribeInfo>(channelName, channelSubscribe));
	}
	m_mutex.unlock();
}

// 此函数是服务器执行的
void IckStormSubscriberSocket::addSubscribe(SocketParam* socketParam, std::string channelName)
{
	std::list<std::string> dataList;
	if (channelName.at(channelName.length() - 1) == '/')
	{
		dataList = ObjectManager::getInstance()->getChannelNodeList(channelName);
	}
	else
	{
		dataList.push_back(channelName);
	}

	std::list<std::string>::iterator it;
	for (it = dataList.begin(); it != dataList.end(); ++it)
	{
		std::string strChannelName = (channelName.at(channelName.length() - 1) == '/') ? channelName + (*it) : (*it);
		UntypedData* untypedData = dynamic_cast<UntypedData*>(
			ObjectManager::getInstance()->getObject(strChannelName));
		if (untypedData == NULL) { std::cout << "subscribe failed exception, for channel " << strChannelName << std::endl; return; }

		this->insertSubscribeChannel(socketParam, untypedData);
		untypedData->subscribe(this);

		// 首次订阅，直接发布数据
		// this->update(untypedData);
		/*
		m_mutex.lock();
		ChannelSubscribeInfo channelInfo;
		channelInfo.channelName = strChannelName;
		channelInfo.registTime = time(0);
		channelInfo.isRegistSuccess = true;

		SocketSubscribeChannelMap::iterator it;

		if ((it = m_clientSocketMap.find(socketParam)) == m_clientSocketMap.end())
		{
			ChannelSubscribeInfoMap channelInfoMap;
			channelInfoMap.insert(std::pair<UntypedData*, ChannelSubscribeInfo>(untypedData, channelInfo));
			m_clientSocketMap.insert(std::pair<SocketParam*, ChannelSubscribeInfoMap>(socketParam, channelInfoMap));
		}
		else
		{
			if (it->second.find(untypedData) == it->second.end())
			{
				it->second.insert(std::pair<UntypedData*, ChannelSubscribeInfo>(untypedData, channelInfo));
			}
		}
		m_mutex.unlock();
		*/
		
	}
}

void IckStormSubscriberSocket::addSendData(std::string data)
{
	if (data.empty()) return;
	this->m_dataSendList.push_back(data);
}

void IckStormSubscriberSocket::update(UntypedData* untypedData)
{
	std::string channelName = untypedData->getFullName();
	this->m_mutex.lock();
	
	IckCore::RemoteValueInfo valueInfo;
	valueInfo.timeStamp = time(0);
	valueInfo.dValue = 0;
	valueInfo.iValue = 0;
	valueInfo.sValue = "";
	long long registeTimeSpan = 0;

	SocketSubscribeChannelMap::iterator itSocket;
	for (itSocket = m_clientSocketMap.begin(); itSocket != m_clientSocketMap.end(); )
	{
		if (itSocket->first->isAlive == false) {
			std::cout << "socket error: isAlive is false!" << std::endl; ++itSocket; continue;
		}

		//long long registeTimeSpan = 0;

		ChannelSubscribeInfoMap* channelSubscribeMap = &itSocket->second;
		ChannelSubscribeInfoMap::iterator itChannel;
		if ((itChannel = channelSubscribeMap->find(untypedData)) == channelSubscribeMap->end())
		{
			++itSocket; continue;
		}
		//registeTimeSpan = valueInfo.timeStamp - itChannel->second.registTime;
		//if (registeTimeSpan > 5 && !untypedData->isChanged()) { ++itSocket; continue; }
		//if (registeTimeSpan > 5) std::cout << "update: " << channelName << " value: " << untypedData->getValueAsString() << std::endl;

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
			valueInfo.iValue = iData->m_value;
			valueInfo.descriptor = "UnInit";
		}
		else if (dData != NULL)
		{
			valueInfo.accessMode = IckCore::READONLY;
			valueInfo.dataType = IckCore::DOUBLEDATA;
			valueInfo.dValue = dData->m_value;
		}
		else if (wiData != NULL)
		{
			valueInfo.accessMode = IckCore::READWRITE;
			valueInfo.dataType = IckCore::INTDATA;
			valueInfo.iValue = wiData->m_value;
		}
		else if (wdData != NULL)
		{
			valueInfo.accessMode = IckCore::READWRITE;
			valueInfo.dataType = IckCore::DOUBLEDATA;
			valueInfo.dValue = wdData->m_value;
		}
		else if (sData != NULL)
		{
			valueInfo.accessMode = IckCore::READONLY;
			valueInfo.dataType = IckCore::STRINGDATA;
			valueInfo.sValue = sData->m_value;
		}
		else if (wsData != NULL)
		{
			valueInfo.accessMode = IckCore::READWRITE;
			valueInfo.dataType = IckCore::STRINGDATA;
			valueInfo.sValue = wsData->m_value;
		}
		int socketErrorId = this->publish(itSocket->first, untypedData->getFullName(), valueInfo);
		if (socketErrorId < 0)   // 发送失败
		{
			struct in_addr ipAddr;
#ifdef __windows__
			memcpy(&ipAddr, &itSocket->first->socketAddress.sin_addr.S_un.S_addr, 4);
#elif __linux__
			memcpy(&ipAddr, &itSocket->first->socketAddress.sin_addr.s_addr, 4);
#endif
			/*std::cout << "socket send error: id: " << socketErrorId << " socket: " 
				<< inet_ntoa(ipAddr) <<": "<< itSocket->first ->socketAddress.sin_port << std::endl<<std::endl;*/
#ifdef __windows__
			itSocket = m_clientSocketMap.erase(itSocket);
#elif __linux__
			SocketSubscribeChannelMap::iterator tmp = itSocket;
			++tmp;
			this->removeClientSocket(itSocket);
			itSocket = tmp;
#endif
			continue;
		}
		++itSocket;
	}
	this->m_mutex.unlock();
}

int IckStormSubscriberSocket::publish(SocketParam* socketParam, const std::string& channelName, const ::IckCore::RemoteValueInfo& valueInfo)
{
	std::string::size_type pos = 0;
	char name[CHANNEL_MAX_LENGTH] = { 0 };

	for (int i = 0; i < channelName.length(); i++) {
		name[i] = channelName[i] == '/' ? '-' : channelName[i];
	}

	std::string strName = std::string(name);

	std::string str = "Update:" + channelName;
	if (valueInfo.accessMode == IckCore::DEFAULT) str += std::string(":U");
	else if(valueInfo.accessMode == IckCore::READONLY) str += std::string(":R");
	else if(valueInfo.accessMode == IckCore::READWRITE) str += std::string(":W");
	
	if (valueInfo.dataType == IckCore::UNKNOWN) str += std::string(":U");
	else if (valueInfo.dataType == IckCore::INTDATA) str += std::string(":I");
	else if (valueInfo.dataType == IckCore::STRINGDATA) str += std::string(":S");
	else if (valueInfo.dataType == IckCore::DOUBLEDATA) str += std::string(":D");

	if (valueInfo.dataType == IckCore::UNKNOWN) str += std::string(":");
	else if (valueInfo.dataType == IckCore::INTDATA) str += std::string(":") + Converter::convertToString(valueInfo.iValue);
	else if (valueInfo.dataType == IckCore::DOUBLEDATA) str += std::string(":") + Converter::convertToString(valueInfo.dValue);
	else if (valueInfo.dataType == IckCore::STRINGDATA) str += std::string(":") + (/*valueInfo.sValue.empty() ? "?????" : */this->encodeData(valueInfo.sValue));

	str += std::string(":") + Converter::convertToString((long)time(0));
	str += ";";

	return this->send(str, socketParam);
}

void IckStormSubscriberSocket::setHost(const std::string& host)
{
	this->m_host = host;
}

void IckStormSubscriberSocket::setPort(int port)
{
	this->m_port = port;
}
void IckStormSubscriberSocket::setServer(bool isServer)
{
	this->m_isClient = !isServer;
}

std::string IckStormSubscriberSocket::encodeData(std::string data)
{
	std::string res = "";
	for (int i = 0; i < data.length(); i++)
	{
		if (data.at(i) == ':')
		{
			res += "%3A";
		}
		else
		{
			res += data.at(i);
		}
	}
	return res;
}

std::string IckStormSubscriberSocket::decodeData(std::string data)
{
	std::string res = "";
	for (int i = 0; i < data.length(); i++)
	{
		if ( i < data.length() - 2 && 
			data.at(i) == '%' && data.at(i+1) == '3' && data.at(i+2) == 'A')
		{
			res += ":";
			i += 2;
		}
		else
		{
			res += data.at(i);
		}
	}
	return res;
}
