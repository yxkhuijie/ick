#ifndef _READ_ONLY_STRING_H_
#define _READ_ONLY_STRING_H_

#include "src/untyped_data.h"

class ick_api ReadOnlyString : public UntypedData
{

public:

  ReadOnlyString(std::string value="");
  
  virtual ~ReadOnlyString();

private:
  bool m_valid;
  std::string m_value;
  std::string m_oldValue;
  friend class IckStormPublisher;
  friend class IckStormSubscriberManager;
  friend class IckStormSubscriberSocket;
public:

  std::string getValue();

  void setSimulatedValue(const std::string& value);

  /*
  * @ brief get value as string
  * @ return string: string format of value
  */
  virtual std::string getValueAsString() const;

};

#endif

