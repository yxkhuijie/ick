#ifndef _VALUE_INFO_H_
#define _VALUE_INFO_H_

#include "IObject.h"
#include "AccessMode.h"
#include "DataType.h"
#include "IckStormData.h"

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

