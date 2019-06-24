#include "src/ick_service_impl.h"
#include "src/control_object.h"
#include "src/object_manager.h"

IckServiceImpl::IckServiceImpl()
{
}


IckServiceImpl::~IckServiceImpl()
{
}

void IckServiceImpl::runService(const std::string& objectName, const std::string& serviceName, const std::string& param/*, const ::Ice::Current& s*/)
{
  std::cout << "IckServiceImpl::runService: object: " << objectName << "  service: " << serviceName << "  param: " << param << std::endl;
  // objectName = std::string("/Control/PLC/RpRepeater01");
  ControlObject* object = dynamic_cast<ControlObject*>(ObjectManager::getInstance()->getObject(objectName));
  if (object != NULL)
  {
    object->call(serviceName, param);
  }
  else
  {
    std::cout << "--------------------ERROR-----------------"<< std::endl;

  }
}
