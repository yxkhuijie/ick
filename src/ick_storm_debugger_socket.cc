#include "src/ick_storm_debugger_socket.h"
#include "src/converter.h"
#ifdef __linux__
#include <string.h>
#include <arpa/inet.h>
#endif


IckStormDebuggerSocket::IckStormDebuggerSocket()
{
  this->m_isActiveMode = false;
  this->m_interval = 200;
  this->m_timeout = 1000;
  this->m_recvBlockMode = false;
}


IckStormDebuggerSocket::~IckStormDebuggerSocket()
{

}


bool IckStormDebuggerSocket::socketConnectHandler(SocketParam* socketParam)
{
  SOCKET socket = socketParam->socketHandle;

  if (m_isClient)
  {
    char buff[4096];
    memset(buff, 0, sizeof(buff));
    int len = 0;
    std::string strTmp = "";

    while ((len = recv(socket, buff + strTmp.length(), sizeof(buff) - 1 - strTmp.length(), 0)) > 0)
    {
      std::string s = std::string(buff);
      std::vector<std::string> datas = Converter::split(s, ";");
      int dataSize = (len == sizeof(buff) - 1 - strTmp.length()) ? datas.size() - 1 : datas.size();

      for (int i = 0; i < dataSize; i++)
      {
        std::vector<std::string> tmp = Converter::split(datas[i], ":");
        int size = tmp.size();
        if (size > 2 && tmp[0].compare("Debugger") == 0 &&
          tmp[1].compare("GetChannelList") == 0)
        {
          std::string node = tmp[2];
            
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

    if (len == -1)
    {
#ifdef __windows__
      int id = WSAGetLastError();
      if (id == WSAECONNRESET || id == WSAECONNABORTED)
      {
        std::cout << "socket error: id: " << id << " position: 2" << std::endl;
        return false;
      }
      else if (id != WSAETIMEDOUT)
      {
        std::cout << "socket error: id: " << id << std::endl;
      }
#elif __linux__
      return false;
#endif
      // std::cout << "socket error: id: " << id << std::endl;
    }
    else
    {
      std::cout << "socket error: len: 0" << std::endl;
      return false;
    }

    long long t = time(0);
    if (t - this->m_lastHeartBeatTime > 60)
    {
      std::string str = "HeartBeat;";
      int flag = 1;
      if ((flag = ::send(socketParam->socketHandle, str.c_str(), str.length(), 0)) <= 0)
      {
        // socket error
        if (flag == -1)
        {
#ifdef __windows__
          int id = WSAGetLastError();
          if (id == WSAECONNRESET || id == WSAECONNABORTED)
          {
            std::cout << "service socket error: service connection reset or aborted, id: " << id << std::endl;
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
      this->m_lastHeartBeatTime = t;
    }
  }

  return true;
}

void IckStormDebuggerSocket::setHost(const std::string& host)
{
  this->m_host = host;
}

void IckStormDebuggerSocket::setPort(int port)
{
  this->m_port = port;
}
void IckStormDebuggerSocket::setServer(bool isServer)
{
  this->m_isClient = !isServer;
}
