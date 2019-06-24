#include "src/read_only_int.h"
#include "src/mutex.h"
#include <sstream>

ReadOnlyInt::ReadOnlyInt(int value)
{
  m_dataType = INTDATA;
  m_accessMode = READONLY;
  m_value = value;
  m_valueInfo.setValue(value);
  m_oldValue = m_value;
  m_valid = false;
}

ReadOnlyInt::ReadOnlyInt(IntValueInfo& valueInfo)
{
  m_dataType = INTDATA;
  m_accessMode = READONLY;
  m_value = valueInfo.getValue();
  m_valueInfo = valueInfo;
  m_oldValue = m_value;
}


ReadOnlyInt::~ReadOnlyInt()
{

}

int ReadOnlyInt::getValue()
{
  if (isSimulated())
  {
    if (m_value != m_oldValue)
    {
      m_oldValue = m_value;
      m_valid = true;
      m_isChanged = true;
    }

    return m_value;
  }
  if (m_driver != NULL && m_pfn_read.pfn_ri != NULL)
  {
    int value = 0;
    if ((m_driver->*m_pfn_read.pfn_ri)(this->m_ChannelNumber,&value))
    {
      if (m_value != value)
      {
        m_oldValue = m_value;
        m_value = value;
        m_valid = true;
        m_isChanged = true;
      }
      this->m_isValid = true;
    }
  }
  return this->m_value;
}

int ReadOnlyInt::getOldValue()
{
  return this->m_oldValue;
}

bool ReadOnlyInt::isValidated()
{
  return m_valid;
}

void ReadOnlyInt::setSimulatedValue(int value)
{
  if (isSimulated())
  {
    m_value = value;
    this->m_isValid = true;
  }
  else
  {
    throw "read only int data can not be set simulated value in not simulated mode";
  }
}

std::string ReadOnlyInt::getValueAsString() const
{
  std::stringstream s;
  s << m_value << std::endl;

  std::string res = "";
  s >> res;

  return res;
}

IntValueInfo ReadOnlyInt::getValueInfo() const
{
  return m_valueInfo;
}

TypeInfo ReadOnlyInt::getTypeInfo() const
{
  return TypeInfo();
}

