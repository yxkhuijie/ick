#ifndef _CONTROL_OBJECT_H_
#define _CONTROL_OBJECT_H_

#include "src/cmd_target.h"
#include "src/time_converter.h"
#include "src/service.h"
#include <map>


#define DECLARE_SERVICEINFO_LIST \
public: \
  static std::vector<ServiceInfo> m_serviceInfoList;                           \
  virtual void getServiceInfoList(std::vector<ServiceInfo> &v) const;

#define BEGIN_SERVICEINFO_LIST(class_name) \
  std::vector<ServiceInfo> class_name::m_serviceInfoList;                      \
  void class_name::getServiceInfoList(std::vector<ServiceInfo> &v) const            \
  { \
    static INIT_SERVICEINFO_LIST init((ControlObject*)this, class_name::m_serviceInfoList); \
    v = class_name::m_serviceInfoList; \
  }

#define ADD_SERVICE_INFO(class_name, service, description) \
  static INSERT_SERVICE_INFO _insert_##class_name##service(  \
    class_name::m_serviceInfoList, class_name::service::m_info, #service, #description);

#define ADD_PARAM_INFO(class_name, service, paramId, type, description) \
  static INSERT_PARAM_INFO _insert_##class_name##service##paramId( \
    class_name::m_serviceInfoList, class_name::service::m_info, #service, #paramId, #type, #description);

#define END_SERVICEINFO_LIST(class_name) \
  static REG_SERVICEINFO_LIST reg(std::string(#class_name), class_name::m_serviceInfoList);

struct REG_SERVICEINFO_LIST
{
  REG_SERVICEINFO_LIST(const std::string &owner, std::vector<ServiceInfo> &curInfo);
};

// this struct is used to add base control object's services' info to current control object
struct INIT_SERVICEINFO_LIST
{
  INIT_SERVICEINFO_LIST(ControlObject *ctrlObj, std::vector<ServiceInfo> &curInfo);
};

struct INSERT_SERVICE_INFO
{
  INSERT_SERVICE_INFO(std::vector<ServiceInfo> &v, ServiceInfo &info,
    const std::string &service, const std::string &dp);
};

struct INSERT_PARAM_INFO
{
  INSERT_PARAM_INFO(std::vector<ServiceInfo> &v, ServiceInfo &info, const std::string &service,
    const std::string &param, const std::string &type, const std::string &dp);
};


#define SERVICE_DEFINE(service_name)               \
public:                                            \
    class service_name : public Service            \
    {                                              \
      DECLARE_CLASS_DYNAMIC_SERVICE(service_name)      \
    public:                                        \
      virtual void execute();                    \
    };                                             \
    service_name m_##service_name;


#define SERVICE_MAKE(service_name)                 \
  m_##service_name.setServiceName(#service_name);\
    m_##service_name.setOwner(this);

class ick_api ControlObject : public CmdTarget
{

  DECLARE_CLASS_DYNAMIC(ControlObject)
  DECLARE_MESSAGE_LIST

public:
  
  ControlObject(void);

  virtual ~ControlObject(void);

  void setNotSimulated(bool simulated);

  ManagedObject* getObject(const std::string& object_path);

  virtual void initService();

  virtual void make();

  virtual void verify();

  virtual void initialize();

  virtual void startup();

  virtual void shutdown();

  friend struct REG_SERVICEINFO_LIST;

  void sleep(int second);

  void msleep(int millisecond);

private:

  IMutex mutex_service;

  friend class Service;

  #ifdef __windows__
  static DWORD WINAPI ThreadHandler(LPVOID lpParamter);
    #else
  
    #endif

public:

  //static const Service* m_serviceEntity[];

  // static std::map <const std::string, std::vector<ServiceInfo> > m_serviceMap;
  static std::map <const std::string, std::vector<ServiceInfo> >& getServiceMap();

  // static const std::map <const std::string, std::string> m_serviceStr;

  static void registerService(std::string className, ServiceInfo serviceInfo);

  std::map<std::string, Service*> m_services;

  std::string decodeParam(std::string data);

  void addService(std::string name, Service* service);

  void addServiceParam(std::string serviceName, std::string paramName, std::string paramType);

  void call(std::string serviceName, std::string params);

  void call(Service* service);

  void start(Service* service);

};
#endif

