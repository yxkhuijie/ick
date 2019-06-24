#include "src/read_write_double.h"
#include <sstream>
#include <math.h>

ReadWriteDouble::ReadWriteDouble(double value)
{
  this->m_accessMode = READWRITE;
  this->m_dataType = DOUBLEDATA;
  this->m_accuracy = 0.01;
  this->m_value = value;
  this->m_oldValue = 0;
  this->m_isChanged = false;
  this->m_driver = NULL;
}


ReadWriteDouble::~ReadWriteDouble(void)
{
}

ValueInfo ReadWriteDouble::getValueInfo() const
{
  return ValueInfo();
}

TypeInfo ReadWriteDouble::getTypeInfo() const
{
  return TypeInfo();
}

double ReadWriteDouble::getValue()
{
  return m_value;
}

void ReadWriteDouble::setValue(double value)
{
  if (m_driver != NULL && m_pfn_write.pfn_wd != NULL)
  {
    if ((m_driver->*m_pfn_write.pfn_wd)(this->m_ChannelNumber, value))
    {
      if (fabs(m_value - value) >= m_accuracy)
      {
        m_oldValue = m_value;
        m_value = value;
        m_isChanged = true;
      }
      this->m_isValid = true;
    }
  }
  else
  {
    if (fabs(m_value - value) >= m_accuracy)
    {
      m_oldValue = m_value;
      m_value = value;
      m_isChanged = true;
    }
    this->m_isValid = true;
  }
}

std::string ReadWriteDouble::getValueAsString() const
{
  std::stringstream s;
  s << m_value << std::endl;

  std::string res = "";
  s >> res;

  return res;
}
