#ifndef _READ_WRITE_DOUBLE_H_
#define _READ_WRITE_DOUBLE_H_

#include "UntypedData.h"

class ick_api ReadWriteDouble : public UntypedData
{

public:
  
  ReadWriteDouble(double value = 0);

  ~ReadWriteDouble(void);

private: 

  double m_value;

  double m_oldValue;

  friend class IckStormSubscriberSocket;

public:

  /*
  * @ brief get value as string
  * @ return string: string format of value
  */
  virtual std::string getValueAsString() const;

  virtual ValueInfo getValueInfo() const;

  virtual TypeInfo getTypeInfo() const;
  
  double getValue();

  void setValue(double value);
};

#endif

