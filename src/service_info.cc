#include "ServiceInfo.h"



ServiceInfo::ServiceInfo()
{
  m_name = "";
}


ServiceInfo::~ServiceInfo()
{
}

void ServiceInfo::addServiceParam(std::string paramName, std::string paramType)
{
  int paramNum = this->Params.size();
  ServiceParam paramInfo;
  paramInfo.setValue(0);
  paramInfo.setType(paramType);
  paramInfo.setParamIndex(paramNum);
  paramInfo.setParamName(paramName);
  if (Params.find(paramName) == Params.end())
  {
    Params.insert(std::pair<std::string, ServiceParam>(paramName, paramInfo));
    paramNum++;
  }
  else
  {
    Params[paramName] = paramInfo;
  }
}

std::map<std::string, ServiceParam>* ServiceInfo::getParams()
{
  return &this->Params;
}

void ServiceInfo::setName(std::string name)
{
  m_name = name;
}

std::string ServiceInfo::getName() const
{
  return m_name;
}
