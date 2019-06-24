#ifndef _MANAGED_OBJECT_H_
#define _MANAGED_OBJECT_H_

#include "src/object.h"
#include <string>
#include "src/logger.h"

#define CLASS_JOINT(class_name) (&class_name::m_list##class_name)

#define _IMPLEMENT_JOINT(class_name, base_class_name, pNewClass) \
    ClassList class_name::m_joint##class_name = \
    { #class_name, CLASS_JOINT(base_class_name), NULL, pNewClass }; \
    static ClassJoint _insert_##class_name(CLASS_JOINT(class_name)); \
    const ClassJoint* class_name::getJoint() const \
    { return CLASS_JOINT(class_name); } 


#define DECLARE_CLASS_DYNAMIC(class_name)                                            \
public:                                                                        \
  static ClassList m_list##class_name;                                       \
  static std::string m_className##class_name;                                \
  virtual const ClassList* GetList() const;                                  \
    static ManagedObject* CreateObject();                                      \
    static ClassJoint _insert_##class_name;                                    \
    virtual const char* getClassName();


#define IMPLEMENT_CLASS_DYNAMIC(class_name, base_class_name)                         \
  ManagedObject* class_name::CreateObject()                                  \
  {                                                                          \
        class_name* p = new class_name();                                      \
        p->m_className##class_name = #class_name;                              \
    const MSG_ENTITY* msgEntity = class_name::m_msgEntry;\
    return p;                                                              \
  }                                                                          \
                                                                             \
  ClassList class_name::m_list##class_name =                                 \
  {                                                                          \
        #class_name,                                                           \
    &base_class_name::m_list##base_class_name,                             \
    NULL,                                                                  \
    class_name::CreateObject                                               \
  };                                                                         \
                                                                             \
  ClassJoint class_name::_insert_##class_name(&class_name::m_list##class_name);   \
  const ClassList* class_name::GetList() const                               \
  {                                                                          \
    return &class_name::m_list##class_name;                                \
  }                                                                          \
    std::string class_name::m_className##class_name="";                        \
  const char* class_name::getClassName()                       \
  {                                                                          \
    return class_name::m_className##class_name.c_str();                            \
  }
 
  /*
  ManagedObject* class_name::CreateObject() \
  { return new class_name(); } \
    ClassList class_name::m_list##class_name = \
    { #class_name, &class_name::m_list##class_name, NULL, pNewClass }; \
  static ClassJoint _insert_##class_name(CLASS_JOINT(class_name)); \
    const ClassJoint* class_name::getJoint() const \
    { return CLASS_JOINT(class_name); }
  */

class ManagedObject;

struct ClassList
{
    std::string m_className;                 // name of class

    ClassList *m_baseList;                   // joint of parent class

    ClassList *m_nextList;                   // next joint of joint list
    
    static ClassList *m_headerList;          // the header joint of joint list

    ManagedObject* (*m_pCreateObject)();     // pointer of the method which is used to create class instance

    ManagedObject* createObject() const;     // invoke (*m_pCreateObject)() implicit
};

struct ClassJoint
{
  ClassJoint(ClassList *pNewClass);
};


class ick_api ManagedObject : public IObject
{
  DECLARE_CLASS_DYNAMIC(ManagedObject)

public:
  ManagedObject(void);
  ~ManagedObject(void);

private:
  bool m_simulated;
  std::string m_fullName;
  std::string m_selfName;

  void setFullName(std::string fullName);

  friend class ObjectManager;
public:
  virtual void setSimulated(bool simulated);
  bool isSimulated();
  std::string getFullName();
  std::string getSelfName();
};

#endif

