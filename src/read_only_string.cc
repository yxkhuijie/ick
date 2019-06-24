#include "src/read_only_string.h"



ReadOnlyString::ReadOnlyString(std::string value)
{
  this->m_accessMode = READONLY;
  this->m_dataType = STRINGDATA;
  this->m_value = value;
  this->m_oldValue = "";
  this->m_valid = false;
}


ReadOnlyString::~ReadOnlyString()
{
}

std::string ReadOnlyString::getValue()
{
  if (isSimulated())
  {
    if (m_value.compare(m_oldValue) != 0)
    {
      m_oldValue = m_value;
      m_valid = true;
      m_isChanged = true;
    }

    return m_value;
  }
  if (m_driver != NULL && m_pfn_read.pfn_rs != NULL)
  {
    std::string value = "";
    if ((m_driver->*m_pfn_read.pfn_rs)(this->m_ChannelNumber, value))
    {
      if (value.compare(m_value) != 0)
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

void ReadOnlyString::setSimulatedValue(const std::string& value)
{
  if (isSimulated())
  {
    m_value = value;
    this->m_isValid = true;
  }
  else
  {
    throw "set simulated value failed, not in simulated mode!";
  }
}

std::string ReadOnlyString::getValueAsString() const
{
  return m_value;
}
