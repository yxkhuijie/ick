#include "src/grpc_remote_service.h"
#include "src/thread.h"
#include "src/ick_storm_subscriber_socket.h"
#include "src/ick_storm_service_socket.h"
#include "src/ick_storm_debugger_socket.h"

#ifndef _IAPPLICATION_H_
#define _IAPPLICATION_H_

class ick_api IApplication : /*virtual public Ice::Application, */public IThread
{
public:
  IApplication(bool isServer = true, bool isLoadIniConfig = true, bool isLoadXmlConfig = true);
  virtual ~IApplication();

  int main(int argc, char* argv[]);
  int main(int argc, wchar_t* argv[]);
private:

  bool m_isServer;

  bool m_isLoadIniConfig;

  bool m_isLoadXmlConfig;

  /*Ice::CommunicatorPtr m_communicator;*/

  void loadConfig(int argc, char* argv[]);

  std::string m_strConfigFilePath;

  std::string m_strIckServiceEndPoint;

  IckStormSubscriberSocket m_subscriberSocket;

  IckStormServiceSocket m_serviceSocket;

  IckStormDebuggerSocket m_debuggerSocket;

  bool m_isNetworkLan;

  std::string m_strTopicManagerEndPoint;

  std::string m_strSubscriberEndpoint;

  std::string m_subscriberListenHost;

  int m_subscriberListenPort;

  std::string m_serviceListenHost;

  int m_serviceListenPort;

  std::string m_debuggerListenHost;
  
  int m_debuggerListenPort;

  std::string m_strConsoleChartset;

  bool m_isSubscriberEnable;

  bool m_isServiceEnable;

  bool m_isDebuggerEnable;

  std::unique_ptr<GrpcRemoteServiceThread> grpc_remote_service_thread_;

public:

  virtual int run(int, char*[]);
  virtual void execute();

  void runService(std::string objectName, std::string serviceName, std::string params = "");

  //  «∑Ò «æ÷”ÚÕ¯
  bool isNetworkLan();

  void setNetworkType(bool isLAN = true);

  void setSubscriberEnable(bool enable);

  void setServiceEnable(bool enable);

  void setDebuggerEnable(bool enable);

  void setSubscriberListener(std::string host, int port);

  void setServiceListener(std::string host, int port);

  void setServiceEndPoint(std::string endpoint);

  void setTopicManagerEndPoint(std::string endpoint);

  void setSubscriberEndpoint(std::string endpoint);
};

#endif

