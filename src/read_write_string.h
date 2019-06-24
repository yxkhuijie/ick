#ifndef _READ_WRITE_STRING_H_
#define _READ_WRITE_STRING_H_

#include "src/untyped_data.h"

class ick_api ReadWriteString : public UntypedData
{

public:
  
  ReadWriteString(std::string value = "");
  
  virtual ~ReadWriteString();

private:

  std::string m_value;
  std::string m_oldValue;
  friend class IckStormPublisher;
  friend class IckStormSubscriberManager;
  friend class IckStormSubscriberSocket;
public:

  std::string getValue();
  
  void setValue(const std::string& value);

  /*
  * @ brief get value as string
  * @ return string: string format of value
  */
  virtual std::string getValueAsString() const;

};

#endif

