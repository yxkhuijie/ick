#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "ManagedObject.h"
#include "ServiceParam.h"
#include "ServiceInfo.h"
#include <map>
#include <vector>

class ControlObject;

// #define DECLARE_CLASS_DYNAMIC_SERVICE(service_name) \
    // DECLARE_CLASS_DYNAMIC(service_name)
#define DECLARE_CLASS_DYNAMIC_SERVICE(service_name)                                  \
public:                                                                        \
	static ClassList m_list##service_name;                                     \
	virtual const ClassList* GetList() const;                                  \
    static ManagedObject* CreateObject();                                      \
    static ClassJoint _insert_##service_name;                                    \
    static ServiceInfo m_info; \
    virtual const ServiceInfo* getInfo() const;

#define IMPLEMENT_CLASS_DYNAMIC_SERVICE(service_name, class_name)                      \
	ServiceInfo class_name::service_name::m_info;                                \
	ManagedObject* class_name::service_name::CreateObject()                      \
	{                                                                            \
		return new class_name::service_name();                                   \
	}                                                                            \
                                                                                 \
	ClassList class_name::service_name::m_list##service_name =                   \
	{                                                                            \
		#class_name#service_name,                                                \
		&Service::m_listService,                                                 \
		NULL,                                                                    \
		class_name::service_name::CreateObject                                   \
	};                                                                           \
                                                                                 \
	static ClassJoint _insert_##class_name_##service_name(                       \
		&class_name::service_name::m_list##service_name);                        \
		                                                                         \
	const ClassList* class_name::service_name::GetList() const                   \
	{                                                                            \
		return &class_name::service_name::m_list##service_name;                  \
	}                                                                            \
                                                                                 \
    const ServiceInfo* class_name::service_name::getInfo() const         \
	{                                                                            \
		return &m_info;                                            \
    }                                                                            

class ControlObject;

class ick_api Service : public ManagedObject
{
	DECLARE_CLASS_DYNAMIC(Service)

public:

	Service();

	Service(ControlObject* owner);

	~Service();

private:

	ControlObject* m_owner;
	
	/*
	* @ brief service name
	*/
	std::string m_name;

public:

	std::map<std::string, ServiceParam> Params;

	void addServiceParam(std::string paramName, std::string paramType);

public:

	void setOwner(ControlObject* object);
	void setServiceName(std::string name);
	std::string getServiceName();

	ControlObject* getOwner();

	virtual const ServiceInfo* getInfo() const = 0;

	/*
	 * @ brief service execute thread
	 */
	virtual void execute() = 0;

};

#endif // !_SERVICE_H_

