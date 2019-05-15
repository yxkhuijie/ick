#ifndef _READ_WRITE_INT_H_
#define _READ_WRITE_INT_H_

#include "UntypedData.h"

class ick_api ReadWriteInt : public UntypedData
{
public:

  ReadWriteInt(int min = INT_MIN, int max = INT_MAX);

  virtual ~ReadWriteInt();

private:
  int m_value;
  int m_oldValue;
  int m_minValue;
  int m_maxValue;
  friend class IckStormSubscriberSocket;
public:

  /*
  * @ brief get value as string
  * @ return string: string format of value
  */
  virtual std::string getValueAsString() const;


  virtual ValueInfo getValueInfo() const;

  virtual TypeInfo getTypeInfo() const;

  virtual std::string toString();

  int getValue();

  void setValue(int value);
};

#endif

