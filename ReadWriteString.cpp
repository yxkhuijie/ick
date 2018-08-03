#include "ReadWriteString.h"



ReadWriteString::ReadWriteString(std::string value)
{
	this->m_accessMode = READWRITE;
	this->m_dataType = STRINGDATA;
	this->m_value = value;
	this->m_oldValue = "";
}


ReadWriteString::~ReadWriteString()
{
}

std::string ReadWriteString::getValue()
{
	if (m_pfn_read.pfn_rs != NULL && m_driver != NULL)
	{
		std::string value = "";
		if ((m_driver->*m_pfn_read.pfn_rs)(this->m_ChannelNumber, value))
		{
			m_value = value;
		}
	}
	else
	{
		if (this->m_oldValue.compare(this->m_value) != 0)
		{
			this->m_isChanged = true;
			this->m_oldValue = m_value;
		}
	}
	return this->m_value;
}

void ReadWriteString::setValue(const std::string& value)
{
	if (m_driver != NULL && m_pfn_write.pfn_ws != NULL)
	{
		if ((m_driver->*m_pfn_write.pfn_ws)(this->m_ChannelNumber,value))
		{
			if(value.compare(this->m_oldValue) != 0) this->m_isChanged = true;
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

std::string ReadWriteString::getValueAsString() const
{
	return m_value;
}
