#include "ServiceParam.h"

ServiceParam::ServiceParam()
{
  m_intValue = 0;
  m_doubleValue = 0;
  m_stringValue = "";
  m_paramIndex = 0;
  m_paramName = "";
}

ServiceParam::~ServiceParam()
{

}

ServiceParamType ServiceParam::getType()
{
  return this->m_type;
}

void ServiceParam::setType(std::string type)
{
  if (type == "I" || type == "int")
  {
    this->m_type = PARAM_TYPE_INT;
  }
  else if (type == "D" || type == "double")
  {
    this->m_type = PARAM_TYPE_DOUBLE;
  }
  else if (type == "S" || type == "string")
  {
    this->m_type = PARAM_TYPE_STRING;
  }
  else
  {
    throw "add param failure, param type invalid!";
  }
}

int ServiceParam::getIntValue()
{
  return m_intValue;
}

double ServiceParam::getDoubleValue()
{
  return m_doubleValue;
}

std::string ServiceParam::getStringValue()
{
  return m_stringValue;
}

void ServiceParam::setValue(int value)
{
  m_intValue = value;
}

void ServiceParam::setValue(double value)
{
  m_doubleValue = value;
}

void ServiceParam::setValue(std::string value)
{
  m_stringValue = value;
}


void ServiceParam::setParamIndex(int index)
{
  this->m_paramIndex = index;
}

int ServiceParam::getParamIndex()
{
  return this->m_paramIndex;
}

void ServiceParam::setParamName(std::string name)
{
  this->m_paramName = name;
}

std::string ServiceParam::getParamName()
{
  return m_paramName;
}
