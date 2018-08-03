#include "TypeInfo.h"


TypeInfo::TypeInfo()
{
	m_dataType = UNKNOWN;
	m_accessMode = DEFAULT;
}

TypeInfo::TypeInfo(DataType dataType, AccessMode accessMode)
{
	m_dataType = dataType;
	m_accessMode = accessMode;
}

TypeInfo::~TypeInfo()
{

}

DataType TypeInfo::getDataType()
{
	return m_dataType;
}
AccessMode TypeInfo::getAccessMode()
{
	return m_accessMode;
}
