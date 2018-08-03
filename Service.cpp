#include "Service.h"
#include "ControlObject.h"


Service::Service()
{
	this->m_owner = NULL;
}

Service::Service(ControlObject* owner)
{
	this->setOwner(owner);
}

Service::~Service()
{

}

void Service::setOwner(ControlObject* object)
{
	object->addService(this->m_name, this);
    this->m_owner = object;
}

ControlObject* Service::getOwner()
{
	return this->m_owner;
}

void Service::setServiceName(std::string name)
{
	this->m_name = name;
}
std::string Service::getServiceName()
{
	return this->m_name;
}

void Service::addServiceParam(std::string paramName, std::string paramType)
{
	ServiceParam paramInfo;
	paramInfo.setValue(0);
	paramInfo.setType(paramType);
	paramInfo.setParamName(paramName);
	std::map<std::string, Service*>::iterator it;

	if (Params.find(paramName) == it->second->Params.end())
	{
		Params.insert(std::pair<std::string, ServiceParam>(paramName, paramInfo));
	}
	else
	{
		Params[paramName] = paramInfo;
	}
}

ManagedObject* Service::CreateObject()
{
	return NULL;
}

ClassList Service::m_listService =
{
	"Service",
	NULL,
	NULL,
	Service::CreateObject
};

ClassJoint Service::_insert_Service(&Service::m_listService);
const ClassList* Service::GetList() const
{
	return &Service::m_listService;
}

std::string Service::m_classNameService = "Service";                        
const char* Service::getClassName()
{                                                                          
	return Service::m_classNameService.c_str();
}




