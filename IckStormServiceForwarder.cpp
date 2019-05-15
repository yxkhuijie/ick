#include "IckStormServiceForwarder.h"
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
#include <arpa/inet.h>
#endif
IckStormServiceForwarder::IckStormServiceForwarder()
{
  this->m_isClient = true;
  this->m_latestHeartBeatTime = 0;
}


IckStormServiceForwarder::~IckStormServiceForwarder()
{

}

void IckStormServiceForwarder::make()
{

}

void IckStormServiceForwarder::startup()
{
  this->connect();
}

bool IckStormServiceForwarder::socketConnectHandler(SocketParam* socketParam)
{
  if (this->m_isClient)
  {
    char buff[4096];
    memset(buff, 0, sizeof(buff));
    int len = 0;
    std::string strTmp = "";
    int flag = 0;
    while ((flag = len = recv(socketParam->socketHandle, buff + strTmp.length(), sizeof(buff) - 1 - strTmp.length(), 0)) > 0)
    {
      std::string data = "";
      data = std::string(buff);
      std::cout << "Data Received from client: " << inet_ntoa(socketParam->socketAddress.sin_addr) << " : " <<
        socketParam->socketAddress.sin_port << " : " << data.c_str() << std::endl;
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
          if ((flag = ::send(socketParam->socketHandle, sendData.c_str(), sendData.length(), 0)) <= 0)
          {
            std::cout << "service socket send failure: socket: " << socket << std::endl;
            break;
          };
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
          std::cout << "Data format received format error!" << std::endl;
        }

        if (!sendData.empty())
        {
          if ((flag = ::send(socketParam->socketHandle, sendData.c_str(), sendData.length(), 0)) <= 0)
          {
            std::cout << "service socket send failure: socket: " << socket << std::endl;
            break;
          };
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

    if (flag == -1)
    {
#ifdef __windows__
      int id = WSAGetLastError();
      if (id == WSAECONNRESET || id == WSAECONNABORTED)
      {
        return false;
      }
      else if (id != WSAETIMEDOUT)
      {
        std::cout << "service socket error: id: " << id << std::endl;
      }
#elif __linux__
      return false;
#endif
    }
    else
    {
      std::cout << "service socket error: len: 0" << std::endl;
      return false;
    }
  }
  else
  {

  }

  if (this->m_interval > 0) ControlObject::msleep(m_interval);
  return true;
}

void IckStormServiceForwarder::setHost(const std::string& host)
{
  this->m_host = host;
}

void IckStormServiceForwarder::setPort(int port)
{
  this->m_port = port;
}

void IckStormServiceForwarder::setServer(bool isServer)
{
  this->m_isClient = !isServer;
}

void IckStormServiceForwarder::setInterval(int interval)
{
  this->m_interval = interval;
}



IMPLEMENT_CLASS_DYNAMIC(IckStormServiceForwarder, ControlObject)

BEGIN_SERVICEINFO_LIST(IckStormServiceForwarder)
END_SERVICEINFO_LIST(IckStormServiceForwarder)

IMPLEMENT_MESSAGE_LIST(IckStormServiceForwarder, ControlObject)

BEGIN_MESSAGE_LIST(IckStormServiceForwarder)
  SET_S(setHost, IckStormServiceForwarder)
  SET_I(setPort, IckStormServiceForwarder)
  SET_B(setServer, IckStormServiceForwarder)
  SET_I(setInterval, IckStormServiceForwarder)
END_MESSAGE_LIST
