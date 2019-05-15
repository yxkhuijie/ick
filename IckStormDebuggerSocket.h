#ifndef _ICK_STORM_DEBUGGER_SOCKET_H_
#define _ICK_STORM_DEBUGGER_SOCKET_H_

#include "ISocket.h"

class IckStormDebuggerSocket : public ISocket
{
public:

  IckStormDebuggerSocket();

  ~IckStormDebuggerSocket();

public:

  void setHost(const std::string& host);

  void setPort(int port);

  void setServer(bool isServer);

public:

  virtual bool socketConnectHandler(SocketParam* socketParam);

};

#endif
