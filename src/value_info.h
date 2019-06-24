#ifndef _VALUE_INFO_H_
#define _VALUE_INFO_H_

#include "src/object.h"
#include "src/access_mode.h"
#include "src/data_type.h"
#include "src/ick_storm_data.h"

class ick_api ValueInfo : public IObject
{
public:

  ValueInfo();

  ValueInfo(AccessMode accessMode, DataType dataType);

  ValueInfo(IckCore::RemoteAccessMode accessMode, IckCore::RemoteDataType dataType);

  virtual ~ValueInfo();

protected:

  AccessMode m_accessMode;

  DataType m_dataType;

public:

  AccessMode getAccessMode();
  
  DataType getDataType();
};

#endif

