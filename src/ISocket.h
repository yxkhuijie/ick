#ifndef _ISOCKET_H_
#define _ISOCKET_H_

#include "IThread.h"
#include <list>
#include "IMutex.h"
#include "UntypedData.h"
#include <map>

#if defined(__windows__)
  #include <winsock.h>
  #pragma comment(lib, "WS2_32")
#else
typedef int SOCKET;
  #include <sys/socket.h>
  #include <netinet/in.h>
#endif

#define SOCKET_MAX_BUFFER_SIZE 4194304

class ISocket;

struct SocketParam
{
public:
  ISocket*    socketObject;
  SOCKET      socketHandle;
  sockaddr_in socketAddress;
  int         addressLength;
  bool        isAlive;
  IMutex      isAliveMutex;
};

enum SocketType
{
  SOCKET_TCP,
  SOCKET_UDP,
};

struct ChannelSubscribeInfo
{

public:
  std::string channelName;
  long long int registTime;
  bool isRegistSuccess;
  long long int updateTime;
};

typedef std::map<SocketParam*, std::map<UntypedData*, ChannelSubscribeInfo> > SocketSubscribeChannelMap;
typedef std::map<UntypedData*, ChannelSubscribeInfo> ChannelSubscribeInfoMap;


class ick_api ISocket : public IThread
{

public:
  /*
   * @ brief
   * for client, please use this constructor
   */
  ISocket(SocketType type = SOCKET_TCP);

  /*
   * @ brief
   * for client, please use this constructor
   */
  ISocket(std::string& host, int port, bool isClient = true, SocketType type = SOCKET_TCP);


  /*
   * @ brief deconstructor
   */
  virtual ~ISocket();

protected:

  SocketType   m_SocketType;

  /*
   * @ brief
   * indicate this socket is server or client
   * if is server, host will be set 127.0.0.1
   */
  bool         m_isClient;

  /*
   * @ brief
     * for client, this is the socket connected,
   * for server, this is the socket listener.
   */
  // SOCKET       m_socket;
  SocketParam  m_socket;

  /*
   * @ brief
   * for client, this is the server ip address,
   * for server, server ip address, default value is 127.0.0.1
   */
  std::string  m_host;


  /*
   * @ brief
   * for client, this is the server port number, default is 6697,
   * for server, this is the port number for client to client, default is 6697
   */
  int          m_port;

  std::string  m_udpServerHost;
  std::string  m_udpClientHost;

  /*
  * @ brief
  * only for udp client, this is the server port number, default is 6697,
  */
  int          m_udpServerPort;
  int          m_udpClientPort;

  /*
   * @ brief
   * only used for server, this is max number of client to connect
   */
  int          m_clientMaxNum;

  sockaddr_in  m_udpSockAddrServer;

  sockaddr_in  m_udpSockAddrClient;

  /*
   * @ brief sockect connection mode, active mode or passive mode,
   * if avtive mode, socket will send data all the time, response not required,
   * if passive mode, socket will send data first, then recv data, response is 
   * required every time, repeat the action,
   * default value is false (passive mode).
   */
  bool         m_isActiveMode;


  bool         m_recvBlockMode;

  /*
   * @ brief receive data time out, default value is 1000ms (1s) 
   * only valid for active mode.
   */
  int          m_timeout;

  /*
   * @ brief interval time between data sent two times, default 1000ms (1s)
   * only valid for active mode 
   */
  int          m_interval;
  

  // std::list<SocketParam*> m_clientSocketList;


  SocketSubscribeChannelMap  m_clientSocketMap;

  bool m_isDataSimulated;

  long long    m_lastHeartBeatTime;

  bool m_isReconnect;

  /*
   * @ brief if m_isReconnect is true, socket client will be reconnected to server after this value 
   * default value: 1000(ms)
   */
  int m_reconnectTimeInterval;

  bool m_isRecvThreadAlive;
  bool m_isSendThreadAlive;

  SocketParam* m_clientSocketParam;

public:

  IMutex       m_mutex;

public:

  virtual bool connect();

  /*
   * @ brief send data with socket
   * @ param data: data will be send.
   * @ param socket: if 0, m_socket will be used, else, use socket handle you set.
   */
  int send(const std::string& data, SocketParam* socketParam = 0);

  /*
  * @ brief receive data with socket
  * @ param data: data buffer will be received.
  * @ param length: data buffer length.
  * @ param socket: if 0, m_socket will be used, else, use socket handle you set.
  */
  int receive(char* data, int length, SocketParam* socketParam = 0);

  /*
   * @ brief data process proc, you can override this function if you required
   * @ default action: 
       in active mode, this function while 
     1. call function dataSendProc(SOCKET socket), you should overwrite the proc
     2. sleep period time (m_interval)

       in passive mode, this function do this action:
     1. call function dataSendProc(SOCKET socket), you should overwrite the proc
     2. call function dataRecvProc(SOCKET socket, const std::string& data), you should overwrite the proc
     3. sleep period time (m_interval)

   * @ if required, you can override this function
   */
  virtual void dataProcessHandler();

  virtual bool socketConnectHandler(SocketParam* socketParam);

  // data receive handler
  virtual bool socketRecvHandler(SocketParam* socketParam);

  // data send handler
  virtual bool socketSendHandler(SocketParam* socketParam);

  virtual void socketClosedHandler(SocketParam* socketParam);

  void setDataSimulated(bool simulated);

  void setRecvBlockMode(bool isBlock);

private:
  /*
   * @brief socket listener thread, only socket server used.
   */
  virtual void execute();

public:

  void sleep(int second);

  void msleep(int millisecond);

protected:

  virtual void dataSendProc(SocketParam* socketParam);

  /*
   * @brief socket data receiced handler, only socket server used.
   */
  virtual void dataRecvProc(SocketParam* socketParam, const std::string& data);

  void insertClientSocket(SocketParam* socketParam);

  void removeClientSocket(SocketParam* socketParam);
  
  SocketSubscribeChannelMap::iterator removeClientSocket(SocketSubscribeChannelMap::iterator it);

  void insertSubscribeChannel(SocketParam* socketParam, const std::string& channelName);

  void insertSubscribeChannel(SocketParam* socketParam, UntypedData* untypedData);

private:
#ifdef __windows__  
  static DWORD WINAPI dataProcessProc(LPVOID lpParamter);

  static DWORD WINAPI socketConnectProc(LPVOID lpParamter);

  static DWORD WINAPI socketRecvProc(LPVOID lpParameter);

  static DWORD WINAPI socketSendProc(LPVOID lpParameter);
#elif __linux__

#endif
};

#endif
