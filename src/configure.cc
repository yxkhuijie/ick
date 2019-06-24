#include "src/configure.h"
#include <fstream>
#include <iostream>
#include "src/logger.h"
#ifdef __linux__
#include <string.h>
#endif

Configure::Configure()
{
}

Configure::Configure(std::string filePath)
{
  this->loadConfigFile(filePath);
}


Configure::~Configure()
{
}

bool Configure::loadConfigFile(std::string fileName)
{
  if (fileName.empty())
  {
    return "";
  }

  std::ifstream infile;
  infile.open(fileName.c_str());   //将文件流对象与文件连接起来 
  if (!infile.is_open())
  {
    std::string msg = fileName + std::string(" can not be opened! filePath: ") + fileName;
    Logger::getInstance()->Error(msg);
    return false;
  }

  char buff[512];
  std::string section = "";

  while (!infile.eof())
  {
    memset(buff, 0, sizeof(buff));
    infile.getline(buff, sizeof(buff));
    if (buff[0] == '#' || buff[0] == '\0') continue;

    std::string s = buff;
    int index = std::string::npos;
    std::string key = "";
    std::string value = "";

    if (s.at(0) == '[' && s.at(s.length() - 1) == ']')
    {
      section = s.substr(1, s.length() - 2);
    }
    else if ((index = s.find_first_of('=')) != std::string::npos)
    {
      key = s.substr(0, index);
      value = s.substr(index + 1);
    }
    else
    {
      continue;
    }

    std::map<std::string, std::map<std::string, std::string> >::iterator itSection;
    std::map<std::string, std::string>::iterator itKey;
    if (!section.empty() && m_configures.find(section) == m_configures.end())
    {
      std::map<std::string, std::string> values;
      m_configures.insert(std::pair<std::string, std::map<std::string, std::string> >(section, values));
    }

    if ( !key.empty() && (itSection=m_configures.find(section)) != m_configures.end())
    {
      std::string keyName = key;
      while ((index = keyName.find('.')) != std::string::npos)
      {
        keyName = keyName.substr(0, index) + keyName.substr(index + 1, keyName.length() - index - 1);
      }

      if ((itKey = itSection->second.find(keyName)) == itSection->second.end())
      {
        itSection->second.insert(std::pair<std::string, std::string>(keyName, value));
      }
      else
      {
        itKey->second = value;
      }
    }
  }
  infile.close();             //关闭文件输入流 
  return true;
}

std::string Configure::getValueByKey(const std::string section, const std::string key)
{
  std::map<std::string, std::map<std::string, std::string> >::iterator itSection;
  std::map<std::string, std::string>::iterator itKey;
  if ((itSection = m_configures.find(section)) != m_configures.end())
  {
    if ((itKey = itSection->second.find(key)) != itSection->second.end())
    {
      return itKey->second;
    }
  }
  return "";
}
