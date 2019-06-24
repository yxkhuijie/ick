#ifndef _SERVICE_PARAM_H_
#define _SERVICE_PARAM_H_

#include "src/object.h"

enum ServiceParamType
{
  PARAM_TYPE_INT,

  PARAM_TYPE_DOUBLE,
  
  PARAM_TYPE_STRING
};

class ick_api ServiceParam : public IObject
{
public:
  
  ServiceParam();
  
  ~ServiceParam();


private:

  int m_intValue;

  double m_doubleValue;

  std::string m_stringValue;

  ServiceParamType m_type;

  int m_paramIndex;

  std::string m_paramName;

public:
  
  ServiceParamType getType();

  void setType(std::string type);

  int getIntValue();

  double getDoubleValue();

  std::string getStringValue();

  void setValue(int value);

  void setValue(double value);
  
  void setValue(std::string value);

  void setParamIndex(int index);

  int getParamIndex();

  void setParamName(std::string name);

  std::string getParamName();
};

#endif
