#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include "src/object.h"
#include <map>

class ick_api Configure : public IObject
{

public:

  Configure();

  Configure(std::string filePath);

  ~Configure();

private:

  std::string m_fileName;

  std::map<std::string, std::map<std::string, std::string> > m_configures;

public:

  // 
  bool loadConfigFile(std::string fileName);

  // 
  std::string getValueByKey(const std::string section, const std::string key);


};

#endif

