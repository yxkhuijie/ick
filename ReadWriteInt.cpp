#include "ReadWriteInt.h"
#include <sstream>

ReadWriteInt::ReadWriteInt(int min, int max)
{
  this->m_accessMode = READWRITE;
  this->m_dataType = INTDATA;
  this->m_minValue = min;
  this->m_maxValue = max;
  this->m_oldValue = 0;
  this->m_value = 0;
}

ReadWriteInt::~ReadWriteInt()
{
}

std::string ReadWriteInt::getValueAsString() const
{
  std::stringstream s;
  s << m_value << std::endl;

  std::string res = "";
  s >> res;

  return res;
}

ValueInfo ReadWriteInt::getValueInfo() const
{
  return ValueInfo();
}

TypeInfo ReadWriteInt::getTypeInfo() const
{
  return TypeInfo();
}

std::string ReadWriteInt::toString()
{
  return this->getValueAsString();
}

int ReadWriteInt::getValue()
{
  if (m_pfn_read.pfn_ri != NULL && m_driver != NULL)
  {
    int value = 0;
    if ((m_driver->*m_pfn_read.pfn_ri)(this->m_ChannelNumber, &value))
    {
      if (value != this->m_oldValue)
      {
        this->m_isChanged = true;
        this->m_oldValue = m_value;
      }
      m_value = value;
    }
  }
  else
  {
    if (this->m_oldValue != this->m_value)
    {
      this->m_isChanged = true;
      this->m_oldValue = m_value;
    }
  }
  return this->m_value;
}

void ReadWriteInt::setValue(int value)
{
  if (m_pfn_write.pfn_wi != NULL && m_driver != NULL)
  {
    if ((m_driver->*m_pfn_write.pfn_wi)(this->m_ChannelNumber,value))
    {
      if (value != this->m_oldValue) this->m_isChanged = true;
      m_value = value;
      this->m_isValid = true;
    }
  }
  else
  {
    this->m_oldValue = m_value;
    this->m_value = value;
    this->m_isValid = true;
  }
}
