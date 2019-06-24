#ifndef _TYPE_INFO_H_
#define _TYPE_INFO_H_

#include "src/object.h"
#include "src/data_type.h"
#include "src/access_mode.h"

class ick_api TypeInfo : public IObject
{

public:
  
  TypeInfo();

  TypeInfo(DataType dataType, AccessMode accessMode);
  
  virtual ~TypeInfo();

private:

  DataType m_dataType;

  AccessMode m_accessMode;

public:

  DataType getDataType();

  AccessMode getAccessMode();

};

#endif

