#include "IckDataManager.h"



IckDataManager::IckDataManager()
{
}


IckDataManager::~IckDataManager()
{

}

IckDataManager* IckDataManager::m_instance = NULL;

IckDataManager* IckDataManager::getInstance()
{
  if (m_instance == NULL)
  {
    if (m_instance == NULL)
    {
      m_instance = new IckDataManager();
    }
  }
  
  return m_instance;
}
