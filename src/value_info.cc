#include "src/value_info.h"



ValueInfo::ValueInfo()
{
}

ValueInfo::ValueInfo(AccessMode accessMode, DataType dataType)
{

}


ValueInfo::ValueInfo(IckCore::RemoteAccessMode accessMode, IckCore::RemoteDataType dataType)
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
}

ValueInfo::~ValueInfo()
{
}

AccessMode ValueInfo::getAccessMode()
{
  return this->m_accessMode;
}

DataType ValueInfo::getDataType()
{
  return this->m_dataType;
}


