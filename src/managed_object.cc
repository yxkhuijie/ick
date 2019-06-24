#include "ManagedObject.h"
#include <map>



ManagedObject::ManagedObject(void)
{
  this->m_simulated = false;
}


ManagedObject::~ManagedObject(void)
{
}

ClassList *ClassList::m_headerList = NULL;

ManagedObject* ClassList::createObject() const
{
  return this->m_pCreateObject();
}

ClassJoint::ClassJoint(ClassList *pNewClass)
{
  static std::map<std::string, ClassList*> m_classMap;
  if(m_classMap.find(pNewClass->m_className) == m_classMap.end())
  {
      m_classMap.insert(std::pair<std::string, ClassList*>(pNewClass->m_className,pNewClass));
  }

  pNewClass->m_nextList = ClassList::m_headerList;
  ClassList::m_headerList = pNewClass;
}

void ManagedObject::setSimulated(bool simulated)
{
  this->m_simulated = simulated;
}

bool ManagedObject::isSimulated()
{
  return this->m_simulated;
}

void ManagedObject::setFullName(std::string fullName)
{
  this->m_fullName = fullName;
  int index = fullName.find_last_of('/');
  if (index != std::string::npos)
  {
    this->m_selfName = fullName.substr(index + 1);
  }
  else
  {
    this->m_selfName = fullName;
  }
}

std::string ManagedObject::getFullName()
{
  return this->m_fullName;
}

std::string ManagedObject::getSelfName()
{
  return this->m_selfName;
}

/**
 * IMPLEMENT_CLASS_DYNAMIC(class_name, base_class_name)
 */
// IMPLEMENT_CLASS_DYNAMIC(ManagedObject, IObject)


ManagedObject* ManagedObject::CreateObject() 
{ 
  return new ManagedObject(); 
}

ClassList ManagedObject::m_listManagedObject = 
{ 
  "ManagedObject", 
  NULL, 
  NULL, 
  ManagedObject::CreateObject  
};

ClassJoint ManagedObject::_insert_ManagedObject(&ManagedObject::m_listManagedObject);
const ClassList* ManagedObject::GetList() const
{ 
  return &ManagedObject::m_listManagedObject; 
}

std::string ManagedObject::m_classNameManagedObject = "ManagedObject";
const char* ManagedObject::getClassName()
{
  return ManagedObject::m_classNameManagedObject.c_str();
}

