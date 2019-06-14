#ifndef _SERVICE_INFO_H_
#define _SERVICE_INFO_H_

#include "IObject.h"
#include "ServiceParam.h"
#include <map>

class ick_api ServiceInfo : public IObject
{
public:

  ServiceInfo();
  
  ~ServiceInfo();

private:

  std::map<std::string, ServiceParam> Params;

  std::string m_name;

public:

  void addServiceParam(std::string paramName, std::string paramType);

  void setName(std::string name);

  std::string getName() const;

  std::map<std::string, ServiceParam>* getParams();
};

#endif // !_SERVICE_INFO_H_


