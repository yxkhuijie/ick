#include "src/application.h"
#include "src/ick_service_impl.h"
//#include <IceStorm/IceStorm.h>
#include "src/ick_storm_publisher.h"
#include "src/ick_storm_subscriber_manager.h"
#include "src/configure.h"
#include "src/xml_parser.h"
#include "src/logger.h"
#include "src/connection_pool_manager.h"
#include "src/data_base.h"
#include "src/ick_storm_manager.h"
#include "src/converter.h"

IApplication::IApplication(bool isServer, bool isLoadIniConfig, bool isLoadXmlConfig)
{
  IDataBase database;
  this->m_isServer = isServer;
  /*this->m_communicator = NULL;
  this->m_communicator = Ice::initialize();*/
  this->m_isNetworkLan = true; //  �Ǿ�����
  this->m_isLoadIniConfig = isLoadIniConfig;
  this->m_isLoadXmlConfig = isLoadXmlConfig;
  this->m_strConsoleChartset = "65001";
  this->m_isSubscriberEnable = true;
  this->m_isServiceEnable = true;
  this->m_isDebuggerEnable = true;
}


IApplication::~IApplication()
{
  this->stop();
  
  ObjectManager::getInstance()->shutdownNamespace();

  /*if (this->m_communicator) {
    try {
      m_communicator->destroy();
    }
    catch (const Ice::Exception& e) {
      std::cerr << e << std::endl;
    }
  }*/
}

int IApplication::main(int argc, char* argv[])
{
  return this->run(argc, argv);
}

int IApplication::main(int argc, wchar_t* argv[])
{
#ifdef __windows__
  wchar_t szFilePath[MAX_PATH] = { 0 };
  char filePath[MAX_PATH] = { 0 };
  int len = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, filePath, 0, NULL, NULL);
  WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, filePath, len, NULL, NULL);
#endif
  return this->run(argc, NULL);
}

int IApplication::run(int argc, char* argv[])
{
  int status = 0;
  /*Ice::CommunicatorPtr ic = NULL;*/

  try 
  {
    // ������־
    Logger::getInstance()->startup();
    // �������ݿ����ӳع�����
    ConnectionPoolManager::getInstance()->startup();

    wchar_t szFilePath[MAX_PATH] = { 0 };
    char filePath[MAX_PATH] = { 0 };
    std::string strFilePath = "";
    // ����Ƿ�����������������ļ������������ƿռ�
    if (argc <= 1)
    {
#ifdef __windows__
#ifdef _UNICODE
      GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
      (wcsrchr(szFilePath, L'\\'))[1] = 0;

      int len = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, filePath, 0, NULL, NULL);
      WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, filePath, len, NULL, NULL);
      strFilePath = std::string(filePath);
#else
      GetModuleFileName(NULL, filePath, sizeof(szFilePath));
      strFilePath = std::string(filePath).substr(0, std::string(filePath).find_last_of("\\")) + "\\";
      int len = MultiByteToWideChar(CP_ACP, 0, strFilePath.c_str(), -1, szFilePath, 0);
      MultiByteToWideChar(CP_ACP, 0, strFilePath.c_str(), -1, szFilePath, len);
#endif
#elif __linux__
      strFilePath = "/tmp/";
#endif
    }

    Configure conf;
    if (m_isLoadIniConfig)
    {
      this->m_strConfigFilePath = strFilePath;
      conf.loadConfigFile(this->m_strConfigFilePath + "icklib.conf");
    }


    std::string strConsoleChartset = conf.getValueByKey("ICK_SERVER", "IckConsoleCharset");
    if (strConsoleChartset.compare("") != 0)
    {
      this->m_strConsoleChartset = strConsoleChartset;
      std::string charset = std::string("chcp ") + strConsoleChartset;
      system(charset.c_str());
    }
    else
    {
      this->m_strConsoleChartset = "65001";
      std::string charset = std::string("chcp 65001");
      system(charset.c_str());
    }
#ifdef __windows__
    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);;
    COORD size = {120, 9999};
    SetConsoleScreenBufferSize(handle_out, size);
    SMALL_RECT rc = {0, 0, 120-1, 30};     // ���ô���λ�úʹ�С  
    SetConsoleWindowInfo(handle_out, 1, &rc);  
    //CloseHandle(handle_out);    //�رձ�׼����豸���  
#endif
    


    //��ʹ�����ӣ�args���Դ�һЩ��ʹ�������������ӳ�ʱʱ�䣬��ʹ���ͻ����ӳص�������  
    
    /*ic = this->m_communicator;*/

    if (this->m_isServer)
    {
      std::string strTopicManagerEndpoint = conf.getValueByKey("ICK_SERVER", "IckTopicManagerEndPoint");
      std::string strPublisherEndpoint = conf.getValueByKey("ICK_SERVER", "IckPublisherEndPoint");
      std::string strSubscriberEndpoint = conf.getValueByKey("ICK_SERVER", "IckSubscriberEndPoint");
      std::string strNetworkType = conf.getValueByKey("ICK_SERVER", "IckNetworkType");
      std::string strSubscriberListenHost = conf.getValueByKey("ICK_SERVER", "IckSubscriberListenHost");
      std::string strSubscriberListenPort = conf.getValueByKey("ICK_SERVER", "IckSubscriberListenPort");
      std::string strServiceListenHost = conf.getValueByKey("ICK_SERVER", "IckServicerListenHost");
      std::string strServiceListenPort = conf.getValueByKey("ICK_SERVER", "IckServiceListenPort");
      std::string strSubscriberUpdateInterval = conf.getValueByKey("ICK_SERVER", "IckSubscriberInterval");
      std::string strDebuggerListenHost = conf.getValueByKey("ICK_SERVER", "IckDebuggerListenHost");
      std::string strDebuggerListenPort = conf.getValueByKey("ICK_SERVER", "IckDebuggerListenPort");
      std::string strServiceEnable = conf.getValueByKey("ICK_SERVER", "IckServiceEnable");
      std::string strSubscriberEnable = conf.getValueByKey("ICK_SERVER", "IckSubscriberEnable");
      std::string strDebuggerEnable = conf.getValueByKey("ICK_SERVER", "IckDebuggerEnable");

      if (strSubscriberUpdateInterval != "") 
        ObjectManager::getInstance()->setInterval(atoi(strSubscriberUpdateInterval.c_str()));
      this->m_isNetworkLan = (strNetworkType.compare("LAN") == 0);

      if (strNetworkType.compare("WAN") == 0)
      {
        if(strSubscriberEnable.compare("1") == 0 || strSubscriberEnable.compare("true") == 0 )
        {
          int subscriberListenPort = atoi(strSubscriberListenPort.c_str());
          this->m_subscriberSocket.setHost(strSubscriberListenHost);
          this->m_subscriberSocket.setPort(subscriberListenPort);
          this->m_subscriberSocket.setServer(true);
          IckStormSubscriberManager::getInstance()->setIckStormSubscriberSocket(&m_subscriberSocket);
          this->m_subscriberSocket.connect();
        }
        if(strServiceEnable.compare("1") == 0 || strServiceEnable.compare("true") == 0 )
        {
          int serviceListenPort = atoi(strServiceListenPort.c_str());
          this->m_serviceSocket.setHost(strServiceListenHost);
          this->m_serviceSocket.setPort(serviceListenPort);
          this->m_serviceSocket.setServer(true);
          this->m_serviceSocket.connect();
        }
        if(strDebuggerEnable.compare("1") == 0 || strDebuggerEnable.compare("true") == 0 )
        {
          int debuggerListenPort = atoi(strDebuggerListenPort.c_str());
          this->m_debuggerSocket.setHost(strDebuggerListenHost);
          this->m_debuggerSocket.setPort(debuggerListenPort);
          this->m_debuggerSocket.setServer(true);
          this->m_debuggerSocket.connect();
        }
      }

      // �������������
      // ������ΪSimplePrinterAdapter������������Ҫ��������ʹ��ȱʡ��Э��(TCP/IP�����˿�Ϊ10000������) 
      // Ice::ObjectAdapterPtr topicManagerAdapter = 
        // ic->createObjectAdapterWithEndpoints("IceStorm/TopicManager", "default -p 20002");
      //Ice::ObjectAdapterPtr topicManagerAdapter = 
      //  ic->createObjectAdapterWithEndpoints("IceStorm/TopicManager", strTopicManagerEndpoint);
      //// ʵ����һ��PrinterI����ΪPrinter�ӿڴ���һ���������  
      //// IceStorm::TopicPrx* topic = IceStorm::Topic();

      //// ������Ԫ���ӵ��������У������������ָ������ΪSimplePrinter������������Ψһȷ��һ������Ԫ  
      ////adapter1->add(object1, ic->stringToIdentity("IceStorm"));
      //IckStormTopicManager* topicManager = new IckStormTopicManager(topicManagerAdapter, strPublisherEndpoint);
      //IceStorm::TopicPrx topicPrx = topicManager->create("IO");
      //topicManagerAdapter->add(topicManager,Ice::stringToIdentity("TopicManager"));

      //// ��Ϣ����������
      //// Ice::ObjectAdapterPtr publishAdapter = ic->createObjectAdapterWithEndpoints("IceStorm/Publisher", "default -p 20003");
      //Ice::ObjectAdapterPtr publishAdapter = ic->createObjectAdapterWithEndpoints("IceStorm/Publisher", strPublisherEndpoint);
      //IckCore::IckPublisherPtr publisherPtr = new IckStormPublisher(topicPrx);


      //Ice::ObjectPrx publishPrx = publishAdapter->add(publisherPtr, Ice::stringToIdentity("Publisher"));
      //IckCore::IckPublisherPrx prx = IckCore::IckPublisherPrx::checkedCast(publishPrx);
      //publishAdapter->activate();
      //// �������������������ĺô��ǿ��Եȵ�������Դ��λ���ٴ���  
      //topicManagerAdapter->activate();


      ///**************************************************************************************************************/
      //// �ͻ���ֻ��Ҫ�˲���
      //// ��ʼ��������������
      //// Ice::ObjectAdapterPtr subscriberAdapter = ic->createObjectAdapterWithEndpoints("SubscriberAdapter", "tcp");
      //Ice::ObjectAdapterPtr subscriberAdapter = ic->createObjectAdapterWithEndpoints("SubscriberAdapter", strSubscriberEndpoint);
      //IckStormSubscriberManager::getInstance()->setSubscriberAdapter(subscriberAdapter);
      //
      //subscriberAdapter->activate();
      //// ��ʼ��TopicManager����
      //// Ice::ObjectPrx obj = ic->stringToProxy("TopicManager:tcp -h localhost -p 20002");
      //Ice::ObjectPrx obj = ic->stringToProxy(std::string("TopicManager:") + strTopicManagerEndpoint);
      //IceStorm::TopicManagerPrx topicManagerPrx = IceStorm::TopicManagerPrx::checkedCast(obj);
      //IckStormSubscriberManager::getInstance()->setTopicManager(topicManagerPrx);
      /**************************************************************************************************************/
    
      
    }
    else
    {
      std::string strNetworkType = m_isLoadIniConfig ? conf.getValueByKey("ICK_CLIENT", "IckNetworkType") : (m_isNetworkLan ? "LAN" : "WAN");
      std::string strSubscriberListenHost = m_isLoadIniConfig ? conf.getValueByKey("ICK_CLIENT", "IckSubscriberListenHost") : m_subscriberListenHost;
      std::string strSubscriberListenPort = m_isLoadIniConfig ? conf.getValueByKey("ICK_CLIENT", "IckSubscriberListenPort") : Converter::convertToString(this->m_subscriberListenPort);
      std::string strServiceListenHost = m_isLoadIniConfig ? conf.getValueByKey("ICK_CLIENT", "IckServiceListenHost") : m_serviceListenHost;
      std::string strServiceListenPort = m_isLoadIniConfig ? conf.getValueByKey("ICK_CLIENT", "IckServiceListenPort") : Converter::convertToString(this->m_serviceListenPort);
      std::string strDebuggerListenHost = m_isLoadIniConfig ? conf.getValueByKey("ICK_CLIENT", "IckDebuggerListenHost") : m_debuggerListenHost;
      std::string strDebuggerListenPort = m_isLoadIniConfig ? conf.getValueByKey("ICK_CLIENT", "IckDebuggerListenPort") : Converter::convertToString(this->m_debuggerListenPort);
      std::string strServiceEnable = m_isLoadIniConfig ? conf.getValueByKey("ICK_SERVER", "IckServicerEnable") : (this->m_isServiceEnable?"true":"false");
      std::string strSubscriberEnable = m_isLoadIniConfig ? conf.getValueByKey("ICK_SERVER", "IckSubscriberEnable") : (this->m_isSubscriberEnable?"true":"false");
      std::string strDebuggerEnable = m_isLoadIniConfig ? conf.getValueByKey("ICK_SERVER", "IckDebuggerEnable") : (this->m_isDebuggerEnable?"true":"false");

      this->m_isNetworkLan = (strNetworkType.compare("LAN") == 0);
      if (strNetworkType.compare("WAN") == 0)
      {
        if(strSubscriberEnable.compare("1") == 0 || strSubscriberEnable.compare("true") == 0 )
        {
          int subscriberListenPort = m_isLoadIniConfig ? atoi(strSubscriberListenPort.c_str()) : this->m_subscriberListenPort;
          this->m_subscriberSocket.setHost(strSubscriberListenHost);
          this->m_subscriberSocket.setPort(subscriberListenPort);
          this->m_subscriberSocket.setServer(false);
          IckStormSubscriberManager::getInstance()->setIckStormSubscriberSocket(&m_subscriberSocket);
          IckStormManager::getInstance()->setIckStormSubscriberSocket(&m_subscriberSocket);
          this->m_subscriberSocket.connect();
        }
        if(strServiceEnable.compare("1") == 0 || strServiceEnable.compare("true") == 0 )
        {
          int serviceListenPort = m_isLoadIniConfig ? atoi(strServiceListenPort.c_str()) : this->m_serviceListenPort;
          this->m_serviceSocket.setHost(strServiceListenHost);
          this->m_serviceSocket.setPort(serviceListenPort);
          this->m_serviceSocket.setServer(false);
          IckStormManager::getInstance()->setIckStormServiceSocket(&m_serviceSocket);
          this->m_serviceSocket.connect();
        }
        /*
        int debuggerListenPort = m_isLoadConfig ? atoi(strDebuggerListenPort.c_str()) : this->m_debuggerListenPort;
        this->m_debuggerSocket.setHost(strDebuggerListenHost);
        this->m_debuggerSocket.setPort(debuggerListenPort);
        this->m_debuggerSocket.setServer(false);
        this->m_debuggerSocket.connect();
        */
      }
    }

    if (this->m_isLoadXmlConfig)
    {
      XmlParser xmlParser;
      xmlParser.setConfigPath(strFilePath + "config\\");
      xmlParser.loadConfigFile();

      ObjectManager::getInstance()->startNamespace();
    }

    // this->start();
  }
  //��һ��CATCH��׽����ICE����ʱ�׳����쳣��������ͼ�ǣ�����ʱ�����쳣ʱ��ջ����������ص�main�д�ӡ������Ϣ��
  /*catch (const Ice::Exception& e) {
    std::cout << "Exception occured in IApplication::run(): Exception info: ";
    std::cerr << e << std::endl;
    status = 1;
  }*/
  catch (const std::string& msg)
  {
    std::cout << "Exception occured in IApplication::run(): Error msg: " + msg;
    status = 1;
  }
  //��2��catch ��׽�ַ���������������ͼ�ǣ�������ǵĴ�����������������ǿ����׳�һ����������Ϣ���ַ��������ص�main�д�ӡ��Ϣ��
  catch (const char* msg) {
    std::cout << "Exception occured in IApplication::run(): Error msg: ";
    status = 1;
  }

  return status;
}


void IApplication::execute()
{
  int status = 0;
  /*Ice::CommunicatorPtr ic = NULL;*/

  try 
  {
    /*ic = this->m_communicator;
*/

    /*
    * @ brief ����Ƿ��������򴴽�[����������]��������������״̬
    * @       ����ǿͻ��ˣ�����Ե��÷��������ṩ�ķ���
    */
    if (this->m_isServer)
    {
      Configure conf;
      conf.loadConfigFile(this->m_strConfigFilePath + "icklib.conf");
      std::string strServiceEndPoint = conf.getValueByKey("ICK_SERVER", "IckServiceEndPoint");
      std::string strNetworkType = conf.getValueByKey("ICK_SERVER", "IckNetworkType");
      std::string strSubscriberListenPort = conf.getValueByKey("ICK_SERVER", "IckSubscriberListenPort");

      // ����������
      // ������ΪSimplePrinterAdapter������������Ҫ��������ʹ��ȱʡ��Э��(TCP/IP�����˿�Ϊ10000������)  
      // Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("ServiceAdapter", "default -p 20000");
      //Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("ServiceAdapter", strServiceEndPoint);
      //// ʵ����һ��PrinterI����ΪPrinter�ӿڴ���һ���������  
      //Ice::ObjectPtr object = new IckServiceImpl;
      //// ������Ԫ���ӵ��������У������������ָ������ΪSimplePrinter������������Ψһȷ��һ������Ԫ  
      //adapter->add(object, Ice::stringToIdentity("Service"));
      //// �������������������ĺô��ǿ��Եȵ�������Դ��λ���ٴ���  
      //adapter->activate();

      //ic->waitForShutdown();
    }
    else
    {
      Configure conf;
      if (m_isLoadIniConfig)
      {
        conf.loadConfigFile(this->m_strConfigFilePath + "icklib.conf");
      }

      std::string strTopicManagerEndPoint = m_isLoadIniConfig ? conf.getValueByKey("ICK_CLIENT", "IckTopicManagerEndPoint") : m_strTopicManagerEndPoint;
      this->m_strIckServiceEndPoint = m_isLoadIniConfig ? conf.getValueByKey("ICK_CLIENT", "IckServiceEndPoint") : m_strIckServiceEndPoint;
      std::string strSubscriberEndpoint = m_isLoadIniConfig ? conf.getValueByKey("ICK_CLIENT", "IckSubscriberEndPoint") : m_strSubscriberEndpoint;


      // ��ʼ��������������
      
      // Ice::ObjectAdapterPtr subscriberAdapter = ic->createObjectAdapterWithEndpoints("SubscriberAdapter", "tcp");
      //Ice::ObjectAdapterPtr subscriberAdapter = ic->createObjectAdapterWithEndpoints("SubscriberAdapter", strSubscriberEndpoint);
      //IckStormSubscriberManager::getInstance()->setSubscriberAdapter(subscriberAdapter);
      //subscriberAdapter->activate();
      //
      //// ��ʼ��TopicManager����
      //// Ice::ObjectPrx obj = ic->stringToProxy("TopicManager:tcp -h localhost -p 20002");
      //Ice::ObjectPrx obj = ic->stringToProxy(std::string("TopicManager:") + strTopicManagerEndPoint);
      //IceStorm::TopicManagerPrx topicManagerPrx = IceStorm::TopicManagerPrx::checkedCast(obj);
      //IckStormSubscriberManager::getInstance()->setTopicManager(topicManagerPrx);
      //ic->waitForShutdown();
    }
  }
  /*catch (const Ice::Exception& e) {
    std::cout << e.what() << std::endl;
    status = 1;
  }*/
  catch (const char* msg) {
    std::cout << msg << std::endl;
    status = 1;
  }
  
}

void IApplication::loadConfig(int argc, char* argv[])
{
  std::string configPath = "";
  if (argc == 1)
  {
#ifdef __windows__
    wchar_t szFilePath[MAX_PATH] = { 0 };
    ::_wgetcwd(szFilePath, MAX_PATH);

    // GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
    // (_tcsrchr(szFilePath, _T('\\')))[1] = 0;

    char filePath[MAX_PATH] = { 0 };
    int len = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, filePath, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, filePath, len, NULL, NULL);
    // ����·��
    if (configPath.at(1) == ':' || configPath.at(0) == '/')
    {
      configPath = filePath;
    }
    else
    {
      configPath += filePath + std::string("\\config");
    }
#endif
  }

  XmlParser xmlParser;
  xmlParser.setConfigPath(std::string(argv[1]));
  xmlParser.loadConfigFile();
}

void IApplication::runService(std::string objectName, std::string serviceName, std::string params)
{
  //try
  //{
  //  if (this->isNetworkLan())
  //  {
  //    /*Ice::CommunicatorPtr ic = this->m_communicator;*/
  //    // Ice::ObjectPrx base = ic->stringToProxy("Service:default -p 20000");
  //    Ice::ObjectPrx base = ic->stringToProxy(std::string("Service:") + this->m_strIckServiceEndPoint);
  //    IckCore::IckServicePrx ickService = IckCore::IckServicePrx::checkedCast(base);
  //    if (!ickService)
  //    {
  //      throw std::string("Invalid proxy");
  //    }
  //    ickService->runService(objectName, serviceName, params);
  //  }
  //  else
  //  {
  //    this->m_serviceSocket.addService(objectName, serviceName, params);
  //  }
  //}
  //catch (const Ice::Exception& e) {
  //  std::cout << e.what() << std::endl;
  //}
  //catch (const char* msg) {
  //  std::cout << msg << std::endl;
  //}
}

bool IApplication::isNetworkLan()
{
  return m_isNetworkLan;
}

void IApplication::setNetworkType(bool isLAN)
{
  m_isNetworkLan = isLAN;
}

void IApplication::setSubscriberListener(std::string host, int port)
{
  this->m_subscriberListenHost = host;
  this->m_subscriberListenPort = port;
}

void IApplication::setServiceListener(std::string host, int port)
{
  this->m_serviceListenHost = host;
  this->m_serviceListenPort = port;
}

void IApplication::setServiceEndPoint(std::string endpoint)
{
  this->m_strIckServiceEndPoint = endpoint;
}

void IApplication::setTopicManagerEndPoint(std::string endpoint)
{
  this->m_strTopicManagerEndPoint = endpoint;
}

void IApplication::setSubscriberEndpoint(std::string endpoint)
{
  this->m_strSubscriberEndpoint = endpoint;
}

void IApplication::setSubscriberEnable(bool enable)
{
  this->m_isSubscriberEnable = enable;
}

void IApplication::setServiceEnable(bool enable)
{
  this->m_isServiceEnable = enable;
}

void IApplication::setDebuggerEnable(bool enable)
{
  this->m_isDebuggerEnable = enable;
}
