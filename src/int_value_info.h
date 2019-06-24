#ifndef _INT_VALUE_INFO_H_
#define _INT_VALUE_INFO_H_

#include "src/value_info.h"

#ifndef INT_MAX
#define INT_MAX  2147483647
#endif

#ifndef INT_MIN
#define INT_MIN (-2147483647-1)
#endif

class ick_api IntValueInfo : public ValueInfo
{

public:
  
  IntValueInfo();

  IntValueInfo(int minValue, int maxValue);
  
  IntValueInfo(AccessMode accessMode, DataType dataType);
  
  IntValueInfo(IckCore::RemoteAccessMode accessMode, IckCore::RemoteDataType dataType);

  virtual ~IntValueInfo();

private:
  
  /*
   * @ brief value of this data
   */
  int m_value;

  /*
   * @ brief min value of this data
   * @ value: default is INT_MIN (-2147483648)
   */
  int m_minValue;

  /*
  * @ brief max value of this data
  * @ value: default is INT_MAX 2147483647
  */
  int m_maxValue;

  /*
  * @ brief unit of this data
  * @ value: default is empty
  */
  std::string m_unit;

  /*
  * @ brief description of this data
  * @ value: default is empty
  */
  std::string m_description;

  /*
  * @ brief name of this data
  * @ value: default is empty
  *     e.g. for a valve,
  *     you can set the full name: /IO/DigitalOutput/DO01
  *     and set the name is: PV01 for easy to remember.
  */
  std::string m_name;

public:
  
  int getValue();

  void setValue(int value);

  int getMinValue();

  void setMinValue(int value);

  int getMaxValue();

  void setMaxValue(int value);

};

#endif

