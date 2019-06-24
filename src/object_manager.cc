#include "ObjectManager.h"
#include "ControlObject.h"
#include "TimeConverter.h"
#include "IDriver.h"
#include "ReadOnlyDouble.h"
#include "ReadOnlyInt.h"
#include "ReadOnlyString.h"
#include "ReadWriteDouble.h"
#include "ReadWriteInt.h"
#include "ReadWriteString.h"
#include "Logger.h"
#include "Converter.h"

ObjectManager* ObjectManager::m_instance = NULL;

ObjectManager::ObjectManager(void)
{
  m_interval = 200;
  ClassList classList = ControlObject::m_listControlObject;
}


ObjectManager::~ObjectManager(void)
{
  std::map<std::string, NodeName*>::iterator it;
  for (it = m_dataNodeNames.begin(); it != m_dataNodeNames.end(); ++it)
  {
    if (it->second != NULL)
    {
      delete it->second;
    }
  }
  for (it = m_serviceNodeNames.begin(); it != m_serviceNodeNames.end(); ++it)
  {
    if (it->second != NULL)
    {
      delete it->second;
    }
  }
}

ObjectManager* ObjectManager::getInstance()
{
  if(m_instance == NULL)
  {
    m_instance = new ObjectManager;
  }

  return m_instance;
}

void ObjectManager::registerObject(ControlObject* object, std::string fullName)
{
  if(object == NULL) return;
  if(fullName.empty()) return;

  if (fullName.length() > CHANNEL_MAX_LENGTH)
  {
    std::string message = "Error occured in ObjectManager::registerObject: object \"" + fullName + 
      "\" register failed, max length must be shorter than " + Converter::convertToString(CHANNEL_MAX_LENGTH);
    Logger::getInstance()->Fatal(message);
    std::cout << TimeConverter::getCurrentTimeAsStr() << ": " << message << std::endl;
    throw message;
  }

  if(m_entites.find(fullName) == m_entites.end())
  {
    object->setFullName(fullName);
    m_entites.insert(std::pair<std::string, ControlObject*>(fullName, object));
    TIME_DEBUG(fullName + ": registed successful")
  }
  else
  {
    std::string message = "object \"" + fullName + "\" register failed because the full name has been registed!";
    Logger::getInstance()->Error(message);
    throw message;
  }
}

void ObjectManager::registerObject(UntypedData* untypedData, std::string fullName)
{
  if (untypedData == NULL) return;
  if (fullName.compare("") == 0) return;

  if (fullName.at(0) != '/')
  {
    std::string message = "object \"" + fullName + "\" register failed because the full name must begin with \"/\"!";
    Logger::getInstance()->Error(message);
    throw message;
  }

  if (fullName.compare("/") == 0)
  {
    std::string message = "object \"" + fullName + "\" register failed because the full name must not same as \"/\"!";
    Logger::getInstance()->Error(message);
    throw message;
  }

  if (m_datas.find(fullName) == m_datas.end())
  {
    untypedData->setFullName(fullName);
    m_datas.insert(std::pair<std::string, UntypedData*>(fullName, untypedData));
    std::string message = fullName + ": registed successful";
    Logger::getInstance()->Info(message);

    std::vector<std::string> nodeNames = Converter::split(fullName, "/");
    std::string nodeName = "/";
    for (int i = 0; i < nodeNames.size(); i++)
    {
      if(nodeNames[i].compare("") == 0) continue;
      if (this->m_dataNodeNames.find(nodeName) == m_dataNodeNames.end())
      {
        NodeName* node = new NodeName;
        node->addNodeName(nodeNames[i]);
        m_dataNodeNames.insert(std::pair<std::string, NodeName*>(nodeName, node));
      }
      else
      {
        NodeName* node = m_dataNodeNames.find(nodeName)->second;
        node->addNodeName(nodeNames[i]);
      }
      nodeName += nodeNames[i] + "/";
    }
  }
  else
  {
    std::string message = "object \"" + fullName + "\" register failed because the full name has been registed!";
    Logger::getInstance()->Error(message);
    throw message;
  }
}

void ObjectManager::registerObject(std::string className, std::string fullName)
{
  ClassList* classList = ClassList::m_headerList;
  bool result = false;
  while (classList != NULL)
  {
    if (classList->m_className.compare(className) == 0)
    {
      ControlObject* object = dynamic_cast<ControlObject*>(classList->createObject());
      object->setFullName(fullName);
      m_entites.insert(std::pair<std::string, ControlObject*>(fullName, object));
      result = true;
      std::string msg = fullName + std::string(": registed successful");
      Logger::getInstance()->Debug(msg);
      break;
    }
    classList = classList->m_nextList;
  }
  if (result == false)
  {
    std::string message = "object \"" + fullName + "\" register failed because class \""+ className +"\" can not be found!";
    Logger::getInstance()->Error(message);
    throw message;
  }
}

bool ObjectManager::isObjectRegisted(std::string fullName)
{
  if (fullName.empty()) return false;

  std::map<std::string, ControlObject*>::iterator it;
  it = m_entites.find(fullName);

  if (it != m_entites.end())
  {
    return true;
  }
  else
  {
    std::map<std::string, UntypedData*>::iterator it1;
    if ((it1 = m_datas.find(fullName)) != m_datas.end())
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

void ObjectManager::createAlias(ControlObject* object, std::string alias)
{
  if (object == NULL) return;
  if (alias.empty()) return;

  if (m_entites.find(alias) != m_entites.end())
  {
    std::cout << "Create alias for object " << object->getFullName() << " failure! " <<
      "alias: " << object->getFullName() << " has been existed! Error occured in ObjectManager::createAlias()! " << std::endl;
  }
  else
  {
    bool objectExist = false;
    std::map<std::string, ControlObject*>::iterator it;
    for (it = m_entites.begin(); it != m_entites.end(); ++it)
    {
      if (it->second == object)
      {
        objectExist = true;
        break;
      }
    }

    if (!objectExist)
    {
      std::cout << "Create alias for object " << object->getFullName() << " failure! " <<
        "object: " << object->getFullName() << " does not existed! Error occured in ObjectManager::createAlias()! " << std::endl;
    }
  }

  m_entites.insert(std::pair<std::string, ControlObject*>(alias, object));
  TIME_DEBUG(alias + ": createAlias successful! ")
}

void ObjectManager::createAlias(UntypedData* object, std::string alias)
{
  if (object == NULL) return;
  if (alias.empty()) return;

  if (m_datas.find(alias) != m_datas.end())
  {
    std::cout << "Create alias for object " << object->getFullName() << " failure! " <<
      "alias: " << object->getFullName() << " has been existed! Error occured in ObjectManager::createAlias()! " << std::endl;
  }
  else
  {
    bool objectExist = false;
    std::map<std::string, UntypedData*>::iterator it;
    for (it = m_datas.begin(); it != m_datas.end(); ++it)
    {
      if (it->second == object)
      {
        objectExist = true;
        break;
      }
    }

    if (!objectExist)
    {
      std::cout << "Create alias for object " << object->getFullName() << " failure! " <<
        "object: " << object->getFullName() << " does not existed! Error occured in ObjectManager::createAlias()! " << std::endl;
    }
  }

  m_datas.insert(std::pair<std::string, UntypedData*>(alias, object));
  TIME_DEBUG(alias + ": createAlias successful! ")
}

void ObjectManager::createAlias(std::string fullName, std::string alias)
{

}

ManagedObject* ObjectManager::getObject(std::string fullName)
{
  if(fullName.empty()) return NULL;
  
  std::map<std::string, ControlObject*>::iterator it;
  it = m_entites.find(fullName);
  
  if(it != m_entites.end())
  {
    return it->second;
  }
  else
  {
    std::map<std::string, UntypedData*>::iterator it1;
    if ((it1 = m_datas.find(fullName)) != m_datas.end())
    {
      return it1->second;
    }
    else
    {
      std::cout << "Exception occured in ObjectManager::getObject(), object: " << fullName << " not found!";
      return NULL;
    }
  }

}

std::map<std::string, UntypedData*>* ObjectManager::getDatas()
{
  return &m_datas;
}

std::list<std::string> ObjectManager::getChannelNodeList(std::string node)
{
  std::map<std::string, NodeName*>::iterator it;
  if ((it=this->m_dataNodeNames.find(node)) != m_dataNodeNames.end())
  {
    return it->second->getNodeList();
  }
  else
  {
    return std::list<std::string>();
  }
}

std::list<std::string> ObjectManager::getServiceNodeList(std::string node)
{
  std::map<std::string, NodeName*>::iterator it;
  if ((it = this->m_serviceNodeNames.find(node)) != m_serviceNodeNames.end())
  {
    return it->second->getNodeList();
  }
  else
  {
    return std::list<std::string>();
  }
}

void ObjectManager::addNodeInfoCallBack(NodeInfoCallBack callback)
{
  if (callback == NULL) return;
  std::list<NodeInfoCallBack>::iterator it;
  for (it = m_nodeInfoCallBacks.begin(); it != m_nodeInfoCallBacks.end(); ++it)
  {
    if (*it == callback) return;
  }
  this->m_nodeInfoCallBacks.push_back(callback);
}

std::list<NodeInfoCallBack> ObjectManager::getNodeInfoCallBack()
{
  return this->m_nodeInfoCallBacks;
}

std::map<std::string, UntypedData*> ObjectManager::getDatas(int nodeNumber)
{
  std::map<std::string, UntypedData*> res;
  std::map<std::string, UntypedData*>::iterator it;
  for (it = m_datas.begin(); it != m_datas.end(); it++)
  {
    if (it->second->getNodeNumber() != 0 && 
      it->second->getChannelNumber() != 0 &&
      it->second->getNodeNumber() == nodeNumber)
    {
      res.insert(std::pair<std::string, UntypedData*>(it->first, it->second));
    }
  }

  return res;
}

void ObjectManager::startNamespace()
{
  std::map<std::string, ControlObject*>::iterator it;
  for (it = m_entites.begin(); it != m_entites.end(); ++it)
  {
    IDriver* object = dynamic_cast<IDriver*>(it->second);
    if (object != NULL)
    {
      Logger::getInstance()->Debug(object->getFullName() + ": register Channel start");
      object->registChannels();
      Logger::getInstance()->Debug(object->getFullName() + ": register Channel finish");
    }
  }
  int index = 0;
  for (it = m_entites.begin(); it != m_entites.end(); ++it)
  {
    ControlObject* object = dynamic_cast<ControlObject*>(it->second);
    if (object != NULL)
    {
      std::list<ObjectBeginMakeCallBack>::iterator it1;
      for(it1 = this->m_objectBeginMakeCallBacks.begin();
        it1 != this->m_objectBeginMakeCallBacks.end(); ++it1)
      {
        if((*it1)!=NULL) (*it1)(index, m_entites.size(), object->getFullName());
      }

      object->make();
      
      std::list<ObjectAfterMakeCallBack>::iterator it2;
      for(it2 = this->m_objectAfterMakeCallBacks.begin();
        it2 != this->m_objectAfterMakeCallBacks.end(); ++it2)
      {
        if((*it2)!=NULL) (*it2)(index, m_entites.size(), object->getFullName());
      }
      index++;
    }
  }

  index = 0;
  for (it = m_entites.begin(); it != m_entites.end(); ++it)
  {
    ControlObject* object = dynamic_cast<ControlObject*>(it->second);
    if (object != NULL)
    {
      std::list<ObjectBeginVerifyCallBack>::iterator it1;
      for(it1 = this->m_objectBeginVerifyCallBacks.begin();
        it1 != this->m_objectBeginVerifyCallBacks.end(); ++it1)
      {
        if((*it1)!=NULL) (*it1)(index, m_entites.size(), object->getFullName());
      }

      object->verify();

      std::list<ObjectAfterVerifyCallBack>::iterator it2;
      for(it2 = this->m_objectAfterVerifyCallBacks.begin();
        it2 != this->m_objectAfterVerifyCallBacks.end(); ++it2)
      {
        if((*it2)!=NULL) (*it2)(index, m_entites.size(), object->getFullName());
      }
      index++;
    }
  }

  index = 0;
  for (it = m_entites.begin(); it != m_entites.end(); ++it)
  {
    ControlObject* object = dynamic_cast<ControlObject*>(it->second);
    if (object != NULL)
    {
      std::list<ObjectBeginInitCallBack>::iterator it1;
      for(it1 = this->m_objectBeginInitCallBacks.begin();
        it1 != this->m_objectBeginInitCallBacks.end(); ++it1)
      {
        if((*it1)!=NULL) (*it1)(index, m_entites.size(), object->getFullName());
      }

      object->initialize();
      
      std::list<ObjectAfterInitCallBack>::iterator it2;
      for(it2 = this->m_objectAfterInitCallBacks.begin();
        it2 != this->m_objectAfterInitCallBacks.end(); ++it2)
      {
        if((*it2)!=NULL) (*it2)(index, m_entites.size(), object->getFullName());
      }
      index++;
    }
  }

  index = 0;
  for (it = m_entites.begin(); it != m_entites.end(); ++it)
  {
    ControlObject* object = dynamic_cast<ControlObject*>(it->second);
    if (object != NULL)
    {
      std::list<ObjectBeginStartupCallBack>::iterator it1;
      for(it1 = this->m_objectBeginStartupCallBacks.begin();
        it1 != this->m_objectBeginStartupCallBacks.end(); ++it1)
      {
        if((*it1)!=NULL) (*it1)(index, m_entites.size(), object->getFullName());
      }

      object->startup();
      
      std::list<ObjectAfterStartupCallBack>::iterator it2;
      for(it2 = this->m_objectAfterStartupCallBacks.begin();
        it2 != this->m_objectAfterStartupCallBacks.end(); ++it2)
      {
        if((*it2)!=NULL) (*it2)(index, m_entites.size(), object->getFullName());
      }
      index++;
    }
  }

  this->start();
  std::cout << std::endl;
  TIME_DEBUG("Namespace has been startup successfull!\n");
}

void ObjectManager::shutdownNamespace()
{
  std::map<std::string, ControlObject*>::iterator it;
  for (it = m_entites.begin(); it != m_entites.end(); ++it)
  {
    ControlObject* object = dynamic_cast<ControlObject*>(it->second);
    if (object != NULL)
    {
      object->shutdown();
    }
  }

  for (it = m_entites.begin(); it != m_entites.end(); ++it)
  {
    ControlObject* object = dynamic_cast<ControlObject*>(it->second);
    if (object != NULL)
    {
      delete object;
    }
  }

  std::map<std::string, UntypedData*>::iterator it1;
  for (it1 = m_datas.begin(); it1 != m_datas.end(); ++it1)
  {
    UntypedData* object = dynamic_cast<UntypedData*>(it1->second);
    if (object != NULL)
    {
      delete object;
    }
  }
}

void ObjectManager::setInterval(int interval)
{
  if (interval < 0) return;
  m_interval = interval;
}

void ObjectManager::execute()
{
  while (true)
  {
    time_t currentTime = time(0);
    std::map<std::string, UntypedData*>::iterator it1;
    for (it1 = m_datas.begin(); it1 != m_datas.end(); ++it1)
    {
      UntypedData* object = dynamic_cast<UntypedData*>(it1->second);
      if (object->getAccessMode() == READONLY
        && object->getDataType() == INTDATA)
      {
        ReadOnlyInt* data = dynamic_cast<ReadOnlyInt*>(object);
        if (data != NULL) data->getValue();
      }
      else if (object->getAccessMode() == READONLY
        && object->getDataType() == DOUBLEDATA)
      {
        ReadOnlyDouble* data = dynamic_cast<ReadOnlyDouble*>(object);
        if (data != NULL) data->getValue();
      }
      else if (object->getAccessMode() == READONLY
        && object->getDataType() == STRINGDATA)
      {
        ReadOnlyString* data = dynamic_cast<ReadOnlyString*>(object);
        if (data != NULL) data->getValue();
      }
      else if (object->getAccessMode() == READWRITE
        && object->getDataType() == INTDATA)
      {
        ReadWriteInt* data = dynamic_cast<ReadWriteInt*>(object);
        if (data != NULL) data->getValue();
      }
      else if (object->getAccessMode() == READWRITE
        && object->getDataType() == DOUBLEDATA)
      {
        ReadWriteDouble* data = dynamic_cast<ReadWriteDouble*>(object);
        if (data != NULL) data->getValue();
      }
      else if (object->getAccessMode() == READWRITE
        && object->getDataType() == STRINGDATA)
      {
        ReadWriteString* data = dynamic_cast<ReadWriteString*>(object);
        if (data != NULL) data->getValue();
      }
      if (object != NULL && object->isChanged())
      {
        object->update();
      }
    }
#ifdef __windows__
    Sleep(m_interval);
#elif __linux__
    usleep(m_interval*1000);
#endif
  }
}

void ObjectManager::addBeginMakeCallBack(ObjectBeginMakeCallBack callback)
{
  std::list<ObjectBeginMakeCallBack>::iterator it;
  for(it=this->m_objectBeginMakeCallBacks.begin();
    it!=this->m_objectBeginMakeCallBacks.end(); ++it)
  {
    if((*it) == callback) return;
  }
  m_objectBeginMakeCallBacks.push_back(callback);
}

void ObjectManager::addAfterMakeCallBack(ObjectAfterMakeCallBack callback)
{
  std::list<ObjectAfterMakeCallBack>::iterator it;
  for(it=this->m_objectAfterMakeCallBacks.begin();
    it!=this->m_objectAfterMakeCallBacks.end(); ++it)
  {
    if((*it) == callback) return;
  }
  m_objectAfterMakeCallBacks.push_back(callback);
}

void ObjectManager::addBeginInitCallBack(ObjectBeginInitCallBack callback)
{
  std::list<ObjectBeginInitCallBack>::iterator it;
  for(it=this->m_objectBeginInitCallBacks.begin();
    it!=this->m_objectBeginInitCallBacks.end(); ++it)
  {
    if((*it) == callback) return;
  }
  m_objectBeginInitCallBacks.push_back(callback);
}

void ObjectManager::addAfterInitCallBack(ObjectAfterInitCallBack callback)
{
  std::list<ObjectAfterInitCallBack>::iterator it;
  for(it=this->m_objectAfterInitCallBacks.begin();
    it!=this->m_objectAfterInitCallBacks.end(); ++it)
  {
    if((*it) == callback) return;
  }
  m_objectAfterInitCallBacks.push_back(callback);
}

void ObjectManager::addBeginVerifyCallBack(ObjectBeginVerifyCallBack callback)
{
  std::list<ObjectBeginVerifyCallBack>::iterator it;
  for(it=this->m_objectBeginVerifyCallBacks.begin();
    it!=this->m_objectBeginVerifyCallBacks.end(); ++it)
  {
    if((*it) == callback) return;
  }
  m_objectBeginVerifyCallBacks.push_back(callback);
}

void ObjectManager::addAfterVerifyCallBack(ObjectAfterVerifyCallBack callback)
{
  std::list<ObjectAfterVerifyCallBack>::iterator it;
  for(it=this->m_objectAfterVerifyCallBacks.begin();
    it!=this->m_objectAfterVerifyCallBacks.end(); ++it)
  {
    if((*it) == callback) return;
  }
  m_objectAfterVerifyCallBacks.push_back(callback);
}

void ObjectManager::addBeginStartupCallBack(ObjectBeginStartupCallBack callback)
{
  std::list<ObjectBeginStartupCallBack>::iterator it;
  for(it=this->m_objectBeginStartupCallBacks.begin();
    it!=this->m_objectBeginStartupCallBacks.end(); ++it)
  {
    if((*it) == callback) return;
  }
  m_objectBeginStartupCallBacks.push_back(callback);
}

void ObjectManager::addAfterStartupCallBack(ObjectAfterStartupCallBack callback)
{
  std::list<ObjectAfterStartupCallBack>::iterator it;
  for(it=this->m_objectAfterStartupCallBacks.begin();
    it!=this->m_objectAfterStartupCallBacks.end(); ++it)
  {
    if((*it) == callback) return;
  }
  m_objectAfterStartupCallBacks.push_back(callback);
}
