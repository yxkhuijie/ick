#include "src/socket.h"
#include "src/time_converter.h"
#include "src/converter.h"
#include "src/object_manager.h"
#ifdef __linux__
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

ISocket::ISocket(SocketType type)
{
#ifdef __windows__
  WSADATA Data;

  //初始化windows Socket Dll  
  int status = WSAStartup(MAKEWORD(1, 1), &Data);
  if (0 != status)
  {
    std::cout << "Exception occured in ISocket::ISocket, WSAStartup failure!";
    exit(-1);
  }
#elif __linux__

#endif

  m_clientMaxNum = 50;
  m_interval = 1000;
  m_timeout = 0;

  m_isDataSimulated = false;

  // 默认接收阻塞（recv为阻塞模式）
  m_recvBlockMode = true;

  m_socket.addressLength = 0;
  m_socket.isAlive = false;
  m_socket.socketHandle = NULL;
  m_socket.socketObject = this;

  m_lastHeartBeatTime = 0;
  m_SocketType = type;
  m_udpServerHost = "127.0.0.1";
  m_udpClientHost = "127.0.0.1";
  m_udpServerPort = 18030;
  m_udpClientPort = 18031;
  m_isReconnect = true;
  m_reconnectTimeInterval = 1000;
  m_isRecvThreadAlive = false;
  m_isSendThreadAlive = false;
  m_clientSocketParam = NULL;
}

ISocket::ISocket(std::string& host, int port, bool isClient, SocketType type)
{
#ifdef __windows__
  WSADATA Data;

  //初始化windows Socket Dll  
  int status = WSAStartup(MAKEWORD(1, 1), &Data);
  if (0 != status)
  {
    std::cout << "Exception occured in ISocket::ISocket, WSAStartup failure!";
    exit(-1);
  }
#elif __linux__

#endif

  m_clientMaxNum = 50;
  m_interval = 1000;
  m_timeout = 1000;
  m_SocketType = type;
  m_udpClientHost = "127.0.0.1";
}


ISocket::~ISocket()
{
#ifdef __windows__
  WSACleanup();
#elif __linux__

#endif
}

bool ISocket::connect()
{
  if (!m_isClient)
  {
    if(this->m_SocketType == SOCKET_TCP)
    {
      m_socket.socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      struct sockaddr_in sockAddr;
      memset((char *)&sockAddr, 0, sizeof(sockAddr));
#ifdef __windows__
      sockAddr.sin_family = AF_INET;
      sockAddr.sin_addr.S_un.S_addr = inet_addr(m_host.c_str());
      sockAddr.sin_port = htons(m_port);
      sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
#elif __linux__
      sockAddr.sin_family = AF_INET;
      sockAddr.sin_addr.s_addr = inet_addr(m_host.c_str());
      sockAddr.sin_port = htons(m_port);
#endif
      if(bind(m_socket.socketHandle, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) < 0) // return -1(SOCKET_ERROR) when error
      {
#ifdef __windows__
        int errId = ::GetLastError();
        throw std::string("Socket bind error: ") + m_host + ":" + Converter::convertToString(m_port) + ", error id: " + Converter::convertToString(errId);
#elif __linux__
#endif    
      }
      if(listen(m_socket.socketHandle, m_clientMaxNum) < 0) // return -1(SOCKET_ERROR) when error
      {
#ifdef __windows__
        int errId = ::GetLastError();
        throw std::string("Socket listen error: ") + m_host + ":" + Converter::convertToString(m_port) + ", error id: " + Converter::convertToString(errId);
#elif __linux__
#endif    
      }
    }
    else if (this->m_SocketType == SOCKET_UDP)
    {
      m_socket.socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    
    this->start();
  }
  else
  {
    if(this->m_SocketType == SOCKET_TCP)
    {
      m_socket.socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      struct sockaddr_in sockAddr;
      memset((char *)&sockAddr, 0, sizeof(sockAddr));
#ifdef __windows__
      sockAddr.sin_family = AF_INET;
      sockAddr.sin_addr.S_un.S_addr = inet_addr(m_host.c_str());
      sockAddr.sin_port = htons(m_port);
      //sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
#elif __linux__
      sockAddr.sin_family = AF_INET;
      sockAddr.sin_addr.s_addr = inet_addr(m_host.c_str());
      sockAddr.sin_port = htons(m_port);
#endif
      if(::connect(m_socket.socketHandle, (sockaddr*)&sockAddr, sizeof(sockAddr))< 0)
      {
#ifdef __windows__
        int err = GetLastError();
#elif __linux__
        int err = -1;
#endif
        std::string msg = std::string("Socket connet failed! Error code: -1");
        Logger::getInstance()->Error(msg);
        return false;
      }
    }
    else if (this->m_SocketType == SOCKET_UDP)
    {
      m_socket.socketHandle = socket(AF_INET, SOCK_DGRAM, 0);

      // 获取本机IP
      char hostname[128];
      int ret = gethostname(hostname, sizeof(hostname));
      if (ret == -1) {
        return -1;
      }
      struct hostent *hent;
      hent = gethostbyname(hostname);
      if (NULL == hent) {
        return -1;
      }
      //直接取h_addr_list列表中的第一个地址h_addr
      if (this->m_udpClientHost.compare("127.0.0.1") == 0 ||
        this->m_udpClientHost.compare("localhost") == 0)
      {
        unsigned int ip = ntohl(((struct in_addr*)hent->h_addr)->s_addr);
        m_udpClientHost = inet_ntoa(*((struct in_addr*)hent->h_addr));
      }

      memset((char *)&m_udpSockAddrServer, 0, sizeof(m_udpSockAddrServer));
      memset((char *)&m_udpSockAddrClient, 0, sizeof(m_udpSockAddrClient));
#ifdef __windows__
      m_udpSockAddrServer.sin_family = AF_INET;
      m_udpSockAddrServer.sin_addr.S_un.S_addr = inet_addr(m_udpServerHost.c_str());
      m_udpSockAddrServer.sin_port = htons(m_udpServerPort);

      m_udpSockAddrClient.sin_family = AF_INET;
      m_udpSockAddrClient.sin_addr.S_un.S_addr = inet_addr(m_udpClientHost.c_str());
      m_udpSockAddrClient.sin_port = htons(m_udpClientPort);
#elif __linux__
      m_udpSockAddrServer.sin_family = AF_INET;
      m_udpSockAddrServer.sin_addr.s_addr = inet_addr(m_udpServerHost.c_str());
      m_udpSockAddrServer.sin_port = htons(m_udpServerPort);

      m_udpSockAddrClient.sin_family = AF_INET;
      m_udpSockAddrClient.sin_addr.s_addr = inet_addr(m_udpClientHost.c_str());
      m_udpSockAddrClient.sin_port = htons(m_udpClientPort);
#endif
      int res = bind(m_socket.socketHandle, (sockaddr*)&m_udpSockAddrClient, sizeof(m_udpSockAddrClient));
      if (res < 0) 
      {
        std::cout << TimeConverter::getCurrentTimeAsStr() << ": socket bind failed: udp: ip: " << inet_ntoa(m_udpSockAddrClient.sin_addr) << " port: " << m_udpClientPort << std::endl;
        return false; 
      }
      else
      {
        std::cout << TimeConverter::getCurrentTimeAsStr() << ": socket bind success: udp: ip: " << inet_ntoa(m_udpSockAddrClient.sin_addr) << " port: " << m_udpClientPort << std::endl;
      }
      res = ::connect(m_socket.socketHandle, (sockaddr*)&m_udpSockAddrServer, sizeof(m_udpSockAddrServer));


      if (res == -1)
      {
#ifdef __windows__
        int err = GetLastError();
        std::cout << TimeConverter::getCurrentTimeAsStr() << ": socket connect failed: udp: ip: " << inet_ntoa(m_udpSockAddrServer.sin_addr) << " port: " << m_udpServerPort << " GetLastError: "<< err <<std::endl;
#elif __linux__

#endif
        throw std::string("Socket connet failed!");
      }
      else
      {
        std::cout << TimeConverter::getCurrentTimeAsStr() << ": socket connect success: udp: ip: " << inet_ntoa(m_udpSockAddrServer.sin_addr) << " port: " << m_udpServerPort << std::endl;

      }
      
    }
    
    this->start();
  }
  return true;
}

int ISocket::send(const std::string& data, SocketParam* socketParam)
{
  if (socketParam == 0 || socketParam->socketHandle == 0) return false;
  int len = ::send(socketParam->socketHandle, data.c_str(), data.length(), 0);
  if (len == -1)         // -1: SOCKET_ERROR
  {
#ifdef __windows__
    int id = WSAGetLastError();
    if (id == WSAECONNRESET || id == WSAECONNABORTED)
    {
      std::string message = std::string(inet_ntoa(socketParam->socketAddress.sin_addr)) + " : " +
        Converter::convertToString(socketParam->socketAddress.sin_port) + ": Socket send error: error id: " + Converter::convertToString(id);
      // Logger::getInstance()->Error(message);
      return -1;
    }
    else if (!this->m_recvBlockMode && id == WSAETIMEDOUT)
    {
      return 0;
    }
    else if (!this->m_recvBlockMode && id != WSAETIMEDOUT)
    {
      std::string message = std::string(inet_ntoa(socketParam->socketAddress.sin_addr)) + " : " +
        Converter::convertToString(socketParam->socketAddress.sin_port) + ": Socket send error: error id: " + Converter::convertToString(id);
      Logger::getInstance()->Error(message);
      return -1;
    }
#elif __linux__

#endif
  }
  else if (len == 0)
  {
    std::string message = std::string(inet_ntoa(socketParam->socketAddress.sin_addr)) + " : " +
      Converter::convertToString(socketParam->socketAddress.sin_port) + ": Socket send error: data received length :0";
    Logger::getInstance()->Error(message);
    return -1;
  }

  return len;
}

int ISocket::receive(char* data, int length, SocketParam* socketParam)
{
  if (socketParam == 0 || socketParam->socketHandle == 0) return false;
  int len = recv(socketParam->socketHandle, data, length, 0);
  if (len == -1)
  {
#ifdef __windows__
    int id = WSAGetLastError();
    if (id == WSAECONNRESET || id == WSAECONNABORTED)
    {
      socketParam->isAlive = false;
      std::string message = std::string(inet_ntoa(socketParam->socketAddress.sin_addr)) + " : " +
        Converter::convertToString(socketParam->socketAddress.sin_port) + ": Socket receive error: error id: " + Converter::convertToString(id);

      return -1;
    }
    else if (!this->m_recvBlockMode && id == WSAETIMEDOUT)
    {
      return 0;
    }
    else if (!this->m_recvBlockMode && id != WSAETIMEDOUT)
    {
      socketParam->isAlive = false;
      std::string message = std::string(inet_ntoa(socketParam->socketAddress.sin_addr)) + " : " +
        Converter::convertToString(socketParam->socketAddress.sin_port) + ": Socket receive error: error id: " + Converter::convertToString(id);
      Logger::getInstance()->Error(message);

      return -1;
    }
#elif __linux__

#endif
  }
  else if (len == 0)
  {
    socketParam->isAlive = false;
    std::string message = std::string(inet_ntoa(socketParam->socketAddress.sin_addr)) + " : " +
      Converter::convertToString(socketParam->socketAddress.sin_port) + ": Socket receive error: data received length :0";
    Logger::getInstance()->Error(message);
    return -1;
  }
  else
  {
    socketParam->isAlive = true;
    /*std::string message = std::string(inet_ntoa(socketParam->socketAddress.sin_addr)) + " : " +
      Converter::convertToString(socketParam->socketAddress.sin_port) + ": RX: " + Converter::convertToString((unsigned char*)data, len);
    Logger::getInstance()->Debug(message);*/
  }
  return len;
}

void ISocket::execute()
{
  if (m_isClient)
  {
    if (this->m_SocketType == SOCKET_TCP)
    {
      m_socket.socketObject = this;
      m_socket.isAlive = true;
      if (m_timeout > 0 && !this->m_recvBlockMode)
      {
        // 设置接收超时时间，默认为1秒
        setsockopt(m_socket.socketHandle, SOL_SOCKET, SO_RCVTIMEO, (char *)&m_timeout, sizeof(int));

        struct linger linger;
        linger.l_onoff = 1;
        linger.l_linger = 0;
        setsockopt(m_socket.socketHandle, SOL_SOCKET, SO_LINGER, (const char *)&linger, sizeof(linger));
      }
    }
#ifdef __windows__
    HANDLE m_thread = CreateThread(NULL, 0, socketConnectProc, (LPVOID)&m_socket, 0, NULL);
#elif __linux__
#endif
  }
  else
  {
    std::list<HANDLE> threadHandleList;
    // 创建仿真数据线程
    HANDLE m_thread = NULL;
    if (this->m_isDataSimulated)
    {
#ifdef __windows__
      m_thread = CreateThread(NULL, 0, dataProcessProc, (LPVOID)this, 0, NULL);
      threadHandleList.push_back(m_thread);
#elif __linux__

#endif
    }
    
    while (true)
    {
      sockaddr_in clientAddr;
      int len = sizeof(clientAddr);
      SOCKET clientSocket = accept(m_socket.socketHandle, (sockaddr*)&clientAddr, (socklen_t*) &len);

      if (clientSocket != -1)
      {
        SocketParam* param = new SocketParam;

        param->socketObject  = this;
        param->socketHandle  = clientSocket;
        param->socketAddress = clientAddr;
        param->addressLength = len;
        param->isAlive       = true;

        // 此处需要加锁
        this->insertClientSocket(param);

        if (this->m_timeout > 0 && !this->m_recvBlockMode)
        {
          setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&m_timeout, sizeof(int));

          struct linger linger;
          linger.l_onoff = 1;
          linger.l_linger = 0;
          setsockopt(clientSocket, SOL_SOCKET, SO_LINGER, (const char *)&linger, sizeof(linger));
        }

        int keepalive = 1;    // 开启keepalive属性
        int keepidle = 60;    // 如该连接在60秒内没有任何数据往来,则进行探测
        int keepinterval = 5; // 探测时发包的时间间隔为5 秒
        int keepcount = 3;    // 探测尝试的次数。如果第1次探测包就收到响应了,则后2次的不再发。
        setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&keepalive, sizeof(keepalive));
        // 设置KeepAlive参数
        /*
        tcp_keepalive alive_in = { 0 };
        tcp_keepalive alive_out = { 0 };
        alive_in.keepalivetime = 5000;                // 开始首次KeepAlive探测前的TCP空闭时间
        alive_in.keepaliveinterval = 60000;                // 两次KeepAlive探测间的时间间隔
        alive_in.onoff = TRUE;
        unsigned long ulBytesReturn = 0;
        int nRet = WSAIoctl(clientSocket, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
          &alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
        */
#ifdef __windows__
        m_thread = CreateThread(NULL, 0, socketConnectProc, (LPVOID)param, 0, NULL);
        threadHandleList.push_back(m_thread);

        //m_thread = CreateThread(NULL, 0, socketRecvProc, (LPVOID)param, 0, NULL);
        //threadHandleList.push_back(m_thread);
        
        //m_thread = CreateThread(NULL, 0, socketSendProc, (LPVOID)param, 0, NULL);
        //threadHandleList.push_back(m_thread);

        struct in_addr ipAddr;
        memcpy(&ipAddr, &clientAddr.sin_addr.S_un.S_addr, 4);
        std::string message = "[ Connection created from client: " + std::string(inet_ntoa(ipAddr));
        message += " : " + Converter::convertToString(clientAddr.sin_port) + " : ";
        message += TimeConverter::getCurrentTimeAsStr() + " ]";
        // std::cout << "[ Connection created from client: " << inet_ntoa(ipAddr) << " : " << clientAddr.sin_port <<" : " << TimeConverter::getCurrentTimeAsStr() << " ]" << std::endl;
        Logger::getInstance()->Info(message);
#elif __linux__

#endif    
      }
      else
      {
#ifdef __windows__
        int errorId = ::GetLastError();
        throw std::string( "Socket accept error! error id: ") + Converter::convertToString(errorId);
#elif __linux__

#endif    
      }
    }

    std::list<HANDLE>::iterator it;
    for (it = threadHandleList.begin(); it != threadHandleList.end(); it++)
    {
#ifdef __windows__
      if (*it != NULL)
      {
        CloseHandle(*it);
      }
#elif __linux__

#endif
    }
  }
}

#ifdef __windows__
DWORD WINAPI ISocket::dataProcessProc(LPVOID lpParamter)
{
  ISocket* object = (ISocket*)lpParamter;
  int interval = (object == NULL) ? 1000 : object->m_interval;
  while (true)
  {
    object->dataProcessHandler();
    if (interval > 0)
    {
      Sleep(interval);
    }
  }
}

DWORD WINAPI ISocket::socketConnectProc(LPVOID lpParamter)
{
  SocketParam* clientSocket = (SocketParam*)(lpParamter);
  ISocket* parent = clientSocket->socketObject;

  SOCKET socket = clientSocket->socketHandle;
  ISocket* object = clientSocket->socketObject;
  int interval = (object == NULL) ? 1000 : object->m_interval;
  while (true)
  {
    if(!object->socketConnectHandler(clientSocket))
    {
      break;
    }
  }

  // parent->socketClosedHandler(clientSocket);
  
  parent->removeClientSocket(clientSocket);
  /*
  SocketSubscribeChannelMap::iterator it;
  if ((it = parent->m_clientSocketMap.find(clientSocket)) != parent->m_clientSocketMap.end())
  {
    parent->m_mutex.lock();
    parent->m_clientSocketMap.erase(it);
    parent->m_mutex.unlock();
  }
  ::closesocket(clientSocket->socketHandle);
  */
  // std::cout << "[ disconnect from client: " <<
    // inet_ntoa(clientSocket->socketAddress.sin_addr) << " : " <<
    // clientSocket->socketAddress.sin_port << " : " << TimeConverter::getCurrentTimeAsStr() << " ]" << std::endl;

  std::string message = "[ disconnect from client: " + std::string(inet_ntoa(clientSocket->socketAddress.sin_addr));
  message += " : " + Converter::convertToString(clientSocket->socketAddress.sin_port) + " : ";
  message += TimeConverter::getCurrentTimeAsStr() + " ]";
  Logger::getInstance()->Info(message);
  
  if (parent->m_isClient && parent->m_isReconnect)
  {
    int reconnectTime = parent->m_reconnectTimeInterval;
    while (reconnectTime > 0)
    {
      std::cout << "reconnet will be created after " << reconnectTime << " ms";
      if (reconnectTime > 1000) 
      { 
        Sleep(1000); 
        reconnectTime -= 1000; 
      }
      else 
      {
        Sleep(reconnectTime); 
        parent->connect();
        break;
      }

    }
  }
  return 1;
}

DWORD WINAPI ISocket::socketRecvProc(LPVOID lpParameter)
{
  SocketParam* clientSocket = (SocketParam*)(lpParameter);
  ISocket* parent = clientSocket->socketObject;
  parent->m_isRecvThreadAlive = true;

  SOCKET socket = clientSocket->socketHandle;
  ISocket* object = clientSocket->socketObject;
  int interval = (object == NULL) ? 1000 : object->m_interval;
  while (true)
  {
    if (!object->socketSendHandler(clientSocket))
    {
      break;
    }
  }

  parent->socketClosedHandler(clientSocket);
  /*
  SocketSubscribeChannelMap::iterator it;
  if ((it = parent->m_clientSocketMap.find(clientSocket)) != parent->m_clientSocketMap.end())
  {
    parent->m_clientSocketMap.erase(it);
  }
  ::closesocket(clientSocket->socketHandle);
  std::cout << "[ disconnect from client: " <<
    inet_ntoa(clientSocket->socketAddress.sin_addr) << " : " <<
    clientSocket->socketAddress.sin_port << " : " << TimeConverter::getCurrentTimeAsStr() << " ]" << std::endl;

  std::string message = "[ disconnect from client: " + std::string(inet_ntoa(clientSocket->socketAddress.sin_addr));
  message += " : " + Converter::convertToString(clientSocket->socketAddress.sin_port) + " : ";
  message += TimeConverter::getCurrentTimeAsStr() + " ]";
  Logger::getInstance()->WriteLine(message);
  
  if (parent->m_isClient && parent->m_isReconnect)
  {
    int reconnectTime = parent->m_reconnectTimeInterval;
    while (reconnectTime > 0)
    {
      std::cout << "reconnet will be created after " << reconnectTime << " ms";
      if (reconnectTime > 1000)
      {
        Sleep(1000);
        reconnectTime -= 1000;
      }
      else
      {
        Sleep(reconnectTime);
        parent->connect();
        break;
      }
    }
  }
  */
  parent->m_isRecvThreadAlive = false;
  return 1;
}

DWORD WINAPI ISocket::socketSendProc(LPVOID lpParamter)
{
  SocketParam* clientSocket = (SocketParam*)(lpParamter);
  ISocket* parent = clientSocket->socketObject;
  parent->m_isSendThreadAlive = true;

  SOCKET socket = clientSocket->socketHandle;
  ISocket* object = clientSocket->socketObject;
  int interval = (object == NULL) ? 1000 : object->m_interval;
  while (true)
  {
    if (!object->socketRecvHandler(clientSocket))
    {
      break;
    }
  }
  // parent->socketClosedHandler(clientSocket);
  /*
  SocketSubscribeChannelMap::iterator it;
  if ((it = parent->m_clientSocketMap.find(clientSocket)) != parent->m_clientSocketMap.end())
  {
    parent->m_mutex.lock();
    parent->m_clientSocketMap.erase(it);
    parent->m_mutex.unlock();
  }
  ::closesocket(clientSocket->socketHandle);
  std::cout << "[ disconnect from client: " <<
    inet_ntoa(clientSocket->socketAddress.sin_addr) << " : " <<
    clientSocket->socketAddress.sin_port << " : " << TimeConverter::getCurrentTimeAsStr() << " ]" << std::endl;

  std::string message = "[ disconnect from client: " + std::string(inet_ntoa(clientSocket->socketAddress.sin_addr));
  message += " : " + Converter::convertToString(clientSocket->socketAddress.sin_port) + " : ";
  message += TimeConverter::getCurrentTimeAsStr() + " ]";
  Logger::getInstance()->WriteLine(message);

  if (parent->m_isClient && parent->m_isReconnect)
  {
    int reconnectTime = parent->m_reconnectTimeInterval;
    while (reconnectTime > 0)
    {
      std::cout << "reconnet will be created after " << reconnectTime << " ms";
      if (reconnectTime > 1000)
      {
        Sleep(1000);
        reconnectTime -= 1000;
      }
      else
      {
        Sleep(reconnectTime);
        parent->connect();
        break;
      }

    }
  }
  */
  parent->m_isSendThreadAlive = false;
  return 1;
}

#endif

void ISocket::dataProcessHandler()
{
  
}

bool ISocket::socketConnectHandler(SocketParam* socketParam)
{
  if (this->m_isActiveMode)
  {
    this->dataSendProc(socketParam);
  }
  else
  {
    this->dataSendProc(socketParam);

    char buff[1024];
    memset(buff, 0, sizeof(buff));
    int len = recv(socketParam->socketHandle, buff, sizeof(buff), 0);
    if (len != -1)
    {
      this->dataRecvProc(socketParam, std::string(buff));
    }
  }

  if (m_interval > 0)
  {
#ifdef __windows__
    Sleep(m_interval);
#elif __linux__
    usleep(m_interval * 1000);
#endif
  }

  return true;
}

bool ISocket::socketRecvHandler(SocketParam* socketParam)
{
  if (m_interval > 0)
  {
#ifdef __windows__
    Sleep(m_interval);
#elif __linux__
    usleep(m_interval * 1000);
#endif
  }
  return true;
}

bool ISocket::socketSendHandler(SocketParam* socketParam)
{
  if (m_interval > 0)
  {
#ifdef __windows__
    Sleep(m_interval);
#elif __linux__
    usleep(m_interval * 1000);
#endif
  }

  return true;
}

void ISocket::socketClosedHandler(SocketParam* socketParam)
{
  // 等待所有线程退出
  while (this->m_isRecvThreadAlive || this->m_isSendThreadAlive)
  {
#ifdef __windows__
    Sleep(1000);
#elif __linux__
    usleep(1000000);
#endif
    std::string message = "wait for recv thread and send thread quit!";
    // std::cout << TimeConverter::getCurrentTimeAsStr() << ": " << message << std::endl;
    Logger::getInstance()->Info(message);
  }
  SocketSubscribeChannelMap::iterator it;
  if ((it = m_clientSocketMap.find(socketParam)) != m_clientSocketMap.end())
  {
    this->m_mutex.lock();
    m_clientSocketMap.erase(it);
    this->m_mutex.unlock();
  }
#ifdef __windows__
  ::closesocket(socketParam->socketHandle);
#endif
  /*std::cout << "[ disconnect from client: " <<
    inet_ntoa(socketParam->socketAddress.sin_addr) << " : " <<
    socketParam->socketAddress.sin_port << " : " << TimeConverter::getCurrentTimeAsStr() << " ]" << std::endl;*/

  std::string message = "[ disconnect from client: " + std::string(inet_ntoa(socketParam->socketAddress.sin_addr));
  message += " : " + Converter::convertToString(socketParam->socketAddress.sin_port) + " : ";
  message += TimeConverter::getCurrentTimeAsStr() + " ]";
  Logger::getInstance()->Info(message);

  if (m_isClient && m_isReconnect)
  {
    int reconnectTime = m_reconnectTimeInterval;
    while (reconnectTime > 0)
    {
      std::cout << "reconnet will be created after " << reconnectTime << " ms";
      if (reconnectTime > 1000)
      {
#ifdef __windows__
        Sleep(1000);
#elif __linux__
        usleep(1000000);
#endif
        reconnectTime -= 1000;
      }
      else
      {
#ifdef __windows__
        Sleep(reconnectTime);
#elif __linux__
        usleep(reconnectTime * 1000);
#endif
        this->connect();
        break;
      }
    }
  }
}

void ISocket::dataSendProc(SocketParam* socketParam)
{

}

void ISocket::dataRecvProc(SocketParam* socketParam, const std::string& data)
{

}

void ISocket::insertClientSocket(SocketParam* socketParam)
{
  if(socketParam == NULL) return;
  this->m_mutex.lock();
  SocketSubscribeChannelMap::iterator it;
  if((it = m_clientSocketMap.find(socketParam)) == m_clientSocketMap.end())
  {
    ChannelSubscribeInfoMap channelInfoMap;
    this->m_clientSocketMap.insert(std::pair<SocketParam*, ChannelSubscribeInfoMap>(socketParam,channelInfoMap));
  }
  this->m_mutex.unlock();
}

void ISocket::removeClientSocket(SocketParam* socketParam)
{
  if(socketParam == NULL) return;
  this->m_mutex.lock();
  SocketSubscribeChannelMap::iterator it;
  if ((it = m_clientSocketMap.find(socketParam)) != m_clientSocketMap.end())
  {
    this->m_clientSocketMap.erase(it);
  }
  this->m_mutex.unlock();
#ifdef __windows__
  ::closesocket(socketParam->socketHandle);
#endif
}

SocketSubscribeChannelMap::iterator ISocket::removeClientSocket(SocketSubscribeChannelMap::iterator it)
{
  this->m_clientSocketMap.erase(it);
}
void ISocket::insertSubscribeChannel(SocketParam* socketParam, const std::string& channelName)
{
  if(socketParam == NULL) return;
  UntypedData* untypedData = dynamic_cast<UntypedData*>(ObjectManager::getInstance()->getObject(channelName));
  if (untypedData == NULL) 
  {
    std::string msg = std::string("subscribe failed exception, for channel ") + channelName;
    Logger::getInstance()->Error(msg);
    return; 
  }

  ChannelSubscribeInfo channelInfo;
  channelInfo.channelName = channelName;
  channelInfo.registTime = time(0);
  channelInfo.isRegistSuccess = true;
  
  this->m_mutex.lock();
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
  this->m_mutex.unlock();
}

void ISocket::insertSubscribeChannel(SocketParam* socketParam, UntypedData* untypedData)
{
  if(socketParam == NULL || untypedData == NULL) return;

  ChannelSubscribeInfo channelInfo;
  channelInfo.channelName = untypedData->getFullName();
  channelInfo.registTime = time(0);
  channelInfo.isRegistSuccess = true;
  
  this->m_mutex.lock();
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
  this->m_mutex.unlock();
}  

void ISocket::setDataSimulated(bool simulated)
{
  m_isDataSimulated = simulated;
}

void ISocket::setRecvBlockMode(bool isBlock)
{
  m_recvBlockMode = isBlock;
}

void ISocket::sleep(int second)
{
#ifdef __windows__
  Sleep(second * 1000);
#elif __linux__
  sleep(second);
#endif
}

void ISocket::msleep(int millisecond)
{
#ifdef __windows__
  Sleep(millisecond);
#elif __linux__
  usleep(millisecond * 1000);
#endif
}

