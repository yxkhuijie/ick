#include "ReadOnlyDouble.h"
#include <sstream>
#include "IMutex.h"
#include <math.h>

ReadOnlyDouble::ReadOnlyDouble()
{
	this->m_accessMode = READONLY;
	this->m_dataType = DOUBLEDATA;
	this->m_accuracy = 0.01;
}

ReadOnlyDouble::ReadOnlyDouble(double value)
{
	this->m_accessMode = READONLY;
	this->m_dataType = DOUBLEDATA;
	this->m_value = value;
	this->m_accuracy = 0.01;
}


ReadOnlyDouble::~ReadOnlyDouble()
{
}


double ReadOnlyDouble::getValue()
{
	if (m_driver != NULL && m_pfn_read.pfn_ri != NULL)
	{
		double value = 0;
		if ((m_driver->*m_pfn_read.pfn_rd)(this->m_ChannelNumber, &value))
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
	return this->m_value;
}

void ReadOnlyDouble::setSimulatedValue(double value)
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

std::string ReadOnlyDouble::getValueAsString() const
{
	std::stringstream s;
	s << m_value << std::endl;
	
	std::string res = "";
	s >> res;

	return res;
}

ValueInfo ReadOnlyDouble::getValueInfo() const
{
	return ValueInfo();
}

TypeInfo ReadOnlyDouble::getTypeInfo() const
{
	return TypeInfo();
}



