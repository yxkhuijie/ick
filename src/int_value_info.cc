#include "IntValueInfo.h"



IntValueInfo::IntValueInfo()
{
  m_minValue = INT_MIN;
  m_maxValue = INT_MAX;
  m_unit = "";
  m_description = "";
  m_name = "";
}

IntValueInfo::IntValueInfo(int minValue, int maxValue)
{
  m_minValue = minValue;
  m_maxValue = maxValue;
  m_unit = "";
  m_description = "";
  m_name = "";
}

IntValueInfo::IntValueInfo(AccessMode accessMode, DataType dataType)
{
  m_unit = "";
  m_description = "";
  m_name = "";
}


IntValueInfo::IntValueInfo(IckCore::RemoteAccessMode accessMode, IckCore::RemoteDataType dataType)
{
  switch (accessMode)
  {
  case IckCore::DEFAULT:
    m_accessMode = DEFAULT;
    break;
  case IckCore::READONLY:
    m_accessMode = READONLY;
    break;
  case IckCore::READWRITE:
    m_accessMode = READWRITE;
    break;
  default:
    break;
  }

  switch (dataType)
  {
  case IckCore::UNKNOWN:
    m_dataType = UNKNOWN;
    break;
  case IckCore::INTDATA:
    m_dataType = INTDATA;
    break;
  case IckCore::STRINGDATA:
    m_dataType = STRINGDATA;
    break;
  case IckCore::DOUBLEDATA:
    m_dataType = DOUBLEDATA;
    break;
  default:
    break;
  }

  m_minValue = INT_MIN;
  m_maxValue = INT_MAX;
  m_unit = "";
  m_description = "";
  m_name = "";
}


IntValueInfo::~IntValueInfo()
{

}

int IntValueInfo::getValue()
{
  return m_value;
}

void IntValueInfo::setValue(int value)
{
  m_value = value;
}

int IntValueInfo::getMinValue()
{
  return m_minValue;
}

void IntValueInfo::setMinValue(int value)
{
  m_minValue = value;
}

int IntValueInfo::getMaxValue()
{
  return m_maxValue;
}

void IntValueInfo::setMaxValue(int value)
{
  m_maxValue = value;
}
