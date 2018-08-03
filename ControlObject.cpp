#include "ControlObject.h"
#include "IMutex.h"
#include "Logger.h"
#include "Converter.h"
#ifdef __linux__
#include <stdlib.h>
#endif
// std::map<const std::string, std::vector<ServiceInfo> > ControlObject::m_serviceMap;

// const std::map<const std::string, std::string> ControlObject::m_serviceStr;

ControlObject::ControlObject(void)
{
	
}


ControlObject::~ControlObject(void)
{
	
}

void ControlObject::initService()
{

}

void ControlObject::make()
{
	TIME_DEBUG(getFullName() + ": make start")
	TIME_DEBUG(getFullName() + ": make finish")
}

void ControlObject::verify()
{
	TIME_DEBUG(getFullName() + ": verify start")
	TIME_DEBUG(getFullName() + ": verify finish")
}

void ControlObject::initialize()
{
	TIME_DEBUG(getFullName() + ": initialize start")
	TIME_DEBUG(getFullName() + ": initialize finish")
}

void ControlObject::startup()
{
	TIME_DEBUG(getFullName() + ": startup start")
	TIME_DEBUG(getFullName() + ": startup finish")
}

void ControlObject::shutdown()
{
	TIME_DEBUG(getFullName() + ": shutdown start")
	TIME_DEBUG(getFullName() + ": shutdown finish")
}

void ControlObject::sleep(int second)
{
#ifdef __windows__
	Sleep(second * 1000);
#elif __linux__
	sleep(second);
#endif
}

void ControlObject::msleep(int millisecond)
{
#ifdef __windows__
	Sleep(millisecond);
#elif __linux__
	usleep(millisecond * 1000);
#endif
}

void ControlObject::setNotSimulated(bool simulated)
{
	this->setSimulated(!simulated);
}

void ControlObject::call(std::string serviceName, std::string params)
{
	std::map<const std::string, std::vector<ServiceInfo> >& 
		serviceMap =this->getServiceMap();
	std::string className = this->getClassName();
	std::map<const std::string, std::vector<ServiceInfo> >::iterator itServiceMap;

    std::map<std::string, ServiceParam>* serviceParamMap = NULL;
	std::vector<ServiceInfo>::iterator itServiceInfo;
	std::vector<ServiceInfo> serviceInfoList;
	ServiceParam serviceParam;
	if ((itServiceMap = serviceMap.find(className)) != serviceMap.end())
	{
		serviceInfoList = itServiceMap->second;
		for (itServiceInfo = serviceInfoList.begin();
			itServiceInfo != serviceInfoList.end(); ++itServiceInfo)
		{
			if (itServiceInfo->getName().compare(serviceName) == 0)
			{
				serviceParamMap = itServiceInfo->getParams();
				break;
			}
		}
	}

	std::map<std::string, Service*>::iterator itService = m_services.find(serviceName);
	if (itService == m_services.end())
	{
		std::cout<< "Run service failure! service " << serviceName << " can not be found!!" << std::endl;
		return;
	}

	if (serviceParamMap != NULL)
	{
		std::vector<std::string> param = split(params, ",");
		if (serviceParamMap->size() != param.size())
		{
			std::string msg = std::string("Run service failure! param format error! service param size: ") + 
				Converter::convertToString((int)serviceParamMap->size()) + std::string(", param size: ") + Converter::convertToString((int)param.size());
			Logger::getInstance()->Error(msg);
			return;
		}

		for (int i = 0; i < param.size(); i++)
		{
			std::map<std::string, ServiceParam>::iterator  itParam;
			for (itParam = serviceParamMap->begin(); itParam != serviceParamMap->end(); ++itParam)
			{
				if (i == itParam->second.getParamIndex())
				{
					switch (itParam->second.getType())
					{
					case PARAM_TYPE_INT:
						itService->second->Params[itParam->second.getParamName()].setValue(atoi(this->decodeParam(param[i]).c_str()));
						break;
					case PARAM_TYPE_DOUBLE:
						itService->second->Params[itParam->second.getParamName()].setValue(atof(this->decodeParam(param[i]).c_str()));
						break;
					case PARAM_TYPE_STRING:
						itService->second->Params[itParam->second.getParamName()].setValue(this->decodeParam(param[i]));
						break;
					}
					break;
				}
			}
		}
		this->call(itService->second);
	}
	/*
	std::map<std::string, Service*>::iterator it;
	if ((it = m_services.find(serviceName)) != m_services.end())
	{
		std::vector<std::string> param = split(params, ",");
		if (it->second->Params.size() != param.size())
		{
			std::cout << "Run service failure! param format error!";
			return;
		}
		else
		{
			for (int i = 0; i < param.size(); i++)
			{
				std::map<std::string, ServiceParam>::iterator  itParam;
				for (itParam = it->second->Params.begin(); itParam != it->second->Params.end(); ++itParam)
				{
					if (i == itParam->second.getParamIndex())
					{
						switch (itParam->second.getType())
						{
						case ServiceParamType::PARAM_TYPE_INT:
							itParam->second.setValue(atoi(param[i].c_str()));
							break;
						case ServiceParamType::PARAM_TYPE_DOUBLE:
							itParam->second.setValue(atof(param[i].c_str()));
							break;
						case ServiceParamType::PARAM_TYPE_STRING:
							itParam->second.setValue(param[i]);
							break;
						}
						break;
					}
				}
			}
			this->call(it->second);
		}
	}
	*/
}

void ControlObject::call(Service* service)
{
	if (service != NULL) 
	{
		this->mutex_service.lock();
		service->execute();
		this->mutex_service.unlock();
	}
}

void ControlObject::start(Service* service)
{
	if (service != NULL) 
	{
		#ifdef __windows__
			CreateThread(NULL, 0, ThreadHandler, service, 0, NULL);
		#endif
	}
}

#ifdef __windows__
DWORD WINAPI ControlObject::ThreadHandler(LPVOID lpParamter)
{
	Service* service = (Service*)lpParamter;
	if (service != NULL)
	{
		ControlObject* object = service->getOwner();
		if(object != NULL) object->mutex_service.lock();
		service->execute();
		if(object != NULL) object->mutex_service.unlock();
	}
	return 0;
}
#endif

void ControlObject::registerService(const std::string className, ServiceInfo serviceInfo)
{
	IMutex mutex;

	std::map<const std::string, std::vector<ServiceInfo> >& serviceMap = getServiceMap();
	std::map <const  std::string, std::vector<ServiceInfo> >::iterator it1;
	// it1 = ControlObject::m_serviceMap.find(owner);
	
	if ((it1 = serviceMap.find(className)) != serviceMap.end())
	{
		it1->second.push_back(serviceInfo);
	}
	else
	{
		std::vector<ServiceInfo> serviceInfoList;
		serviceInfoList.push_back(serviceInfo);
		serviceMap.insert(std::pair<std::string, std::vector<ServiceInfo> >(className, serviceInfoList));
	}
	/*
	std::map<const  std::string, std::string>::const_iterator it2;
	if ((it2 = m_serviceStr.find(className)) != m_serviceStr.end())
	{
		// it2->second=className;
	}
	else
	{
		// m_serviceStr.insert(std::pair<const std::string, std::string >(className, className));
	}

	ControlObject::m_serviceMap = serviceMap;
	*/
}

std::map <const std::string, std::vector<ServiceInfo> >& ControlObject::getServiceMap()
{
	static std::map<const std::string, std::vector<ServiceInfo> > serviceMap;
	return serviceMap;
}

void ControlObject::addService(std::string name, Service* service)
{
	if (service == NULL) return;
	if (this->m_services.find(name) == m_services.end())
	{
		m_services.insert(std::pair<std::string, Service*>(name, service));
	}
	else
	{
		m_services[name] = service;
	}
}

void ControlObject::addServiceParam(std::string serviceName, std::string paramName, std::string paramType)
{
	std::map<std::string, Service*>::iterator it;
	if ((it=this->m_services.find(serviceName)) != m_services.end())
	{
		it->second->addServiceParam(paramName, paramType);
	}
}

INSERT_SERVICE_INFO::INSERT_SERVICE_INFO(std::vector<ServiceInfo> &v, ServiceInfo &info,
	const std::string &service, const std::string &dp)
{
	std::vector<ServiceInfo>::iterator it;
	bool exist = false;
	for (it = v.begin(); it != v.end(); ++it)
	{
		if (it->getName() == service)
		{
			exist = true;
		}
	}

	if (!exist)
	{
		info.setName(service);
		v.push_back(info);
	}
}

REG_SERVICEINFO_LIST::REG_SERVICEINFO_LIST(const std::string & owner, std::vector<ServiceInfo>& curInfo)
{
	std::map<const std::string, std::vector<ServiceInfo> >& serviceMap = ControlObject::getServiceMap();
	std::map <const std::string, std::vector<ServiceInfo> >::iterator it1;
	// it1 = ControlObject::m_serviceMap.find(owner);
	if (!serviceMap.empty() && (((it1 = serviceMap.find(owner)) != serviceMap.end())))
	{
		it1->second = curInfo;
	}
	else
	{
		serviceMap.insert(std::pair<std::string, std::vector<ServiceInfo> >(owner, curInfo));
	}
	serviceMap = ControlObject::getServiceMap();
}

INIT_SERVICEINFO_LIST::INIT_SERVICEINFO_LIST(ControlObject * ctrlObj, std::vector<ServiceInfo>& curInfo)
{
}

INSERT_PARAM_INFO::INSERT_PARAM_INFO(std::vector<ServiceInfo>& v, ServiceInfo & info, 
	const std::string & service, const std::string & param, const std::string & type, const std::string & dp)
{
	info.setName(service);
	info.addServiceParam(param, type);
	
	std::vector<ServiceInfo>::iterator it;
	for (it = v.begin(); it != v.end(); ++it)
	{
		if (it->getName() == service)
		{
			*it = info;
			return;
		}
	}

	v.push_back(info);
}

std::string ControlObject::decodeParam(std::string data)
{
	std::string res = "";
	for (int i = 0; i < data.length(); i++)
	{
		if (i < data.length() - 2 &&
			data.at(i) == '%' && data.at(i + 1) == '3' && data.at(i + 2) == 'A')
		{
			res += ":";
			i += 2;
		}
		else if (i < data.length() - 2 &&
			data.at(i) == '%' && data.at(i + 1) == '2' && data.at(i + 2) == 'C')
		{
			res += ",";
			i += 2;
		}
		else
		{
			res += data.at(i);
		}
	}
	return res;
}


IMPLEMENT_CLASS_DYNAMIC(ControlObject, CmdTarget)

IMPLEMENT_MESSAGE_LIST(ControlObject, CmdTarget)

BEGIN_MESSAGE_LIST(ControlObject)
	SET_B(setNotSimulated, ControlObject)
END_MESSAGE_LIST
