#include "UntypedData.h"

UntypedData::UntypedData()
{
  this->m_NodeNumber = 0;
  this->m_ChannelNumber = 0;
  this->m_dataType = UNKNOWN;
  this->m_accessMode = DEFAULT;
  this->m_driver = NULL;
  this->m_pfn_read.pfn = NULL;
  this->m_pfn_write.pfn = NULL;
  this->m_isChanged = true;
  this->m_isValid = false;
  this->m_strParamFirst = "";
  this->m_strParamSecond = "";
  this->m_strDescription = "";
}


UntypedData::UntypedData(int nodeNumber, int channelNumber)
{
  this->m_NodeNumber = nodeNumber;
  this->m_ChannelNumber = channelNumber;
  this->m_dataType = UNKNOWN;
  this->m_accessMode = DEFAULT;
  this->m_driver = NULL;
  this->m_pfn_read.pfn = NULL;
  this->m_pfn_write.pfn = NULL;
  this->m_isChanged = true;
  this->m_isValid = false;
  this->m_strParamFirst = "";
  this->m_strParamSecond = "";
  this->m_strDescription = "";
}

UntypedData::~UntypedData()
{

}

DataType UntypedData::getDataType()
{
  return this->m_dataType;
}

AccessMode UntypedData::getAccessMode()
{
  return this->m_accessMode;
}

void UntypedData::setNodeNumber(int nodeNumber)
{
  this->m_NodeNumber = nodeNumber;
}

int UntypedData::getNodeNumber()
{
  return this->m_NodeNumber;
}

void UntypedData::setChannelNumber(int channelNumber)
{
  this->m_ChannelNumber = channelNumber;
}

int UntypedData::getChannelNumber()
{
  return this->m_ChannelNumber;
}


void UntypedData::setFirstParamValue(std::string& value)
{
  this->m_strParamFirst = value;
}

void UntypedData::setSecondParamValue(std::string& value)
{
  this->m_strParamSecond = value;
}

void UntypedData::setDescription(std::string& value)
{
  this->m_strDescription = value;
}

std::string UntypedData::getFirstParamValue()
{
  return this->m_strParamFirst;
}

std::string UntypedData::getSecondParamValue()
{
  return this->m_strParamSecond;
}

std::string UntypedData::getDescription()
{
  return this->m_strDescription;
}

void UntypedData::subscribe(Subscriber* object)
{
  if (object == NULL) return;
  m_mutex.lock();
  this->m_isChanged = true;
  std::list<Subscriber*>::iterator it;
  for (it = m_subscribers.begin(); it != m_subscribers.end(); it++)
  {
    if ((*it) == object)
    {
      m_mutex.unlock();
      return;
    }
  }
  
  m_subscribers.push_back(object);
  m_mutex.unlock();
}

void UntypedData::unsubscribe(Subscriber* object)
{
  m_mutex.lock();
  std::list<Subscriber*>::iterator it;
  for (it = m_subscribers.begin(); it != m_subscribers.end();)
  {
    if ((*it) == object)
    {
      it = m_subscribers.erase(it);
    }
    else
    {
      ++it;
    }
  }
  m_mutex.unlock();
}

void UntypedData::registChannelRead(FUNC_REG pfn, CmdTarget* driver)
{
  if (pfn.pfn == NULL) return;
  this->m_pfn_read = pfn;
  this->m_driver = driver;
}

void UntypedData::registChannelWrite(FUNC_REG pfn, CmdTarget* driver)
{
  if (pfn.pfn == NULL) return;
  this->m_pfn_write = pfn;
  this->m_driver = driver;
}

void UntypedData::update()
{
  m_mutex.lock();
  std::list<Subscriber*>::iterator it;
  for (it = m_subscribers.begin(); it != m_subscribers.end(); it++)
  {
    if ((*it) != NULL)
    {
      (*it)->update(this);
    }
  }
  this->m_isChanged = false;
  
  m_mutex.unlock();
}

bool UntypedData::isChanged()
{
  return m_isChanged;
}

bool UntypedData::isValid()
{
  return m_isValid;
}
