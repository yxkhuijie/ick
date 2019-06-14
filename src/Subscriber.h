#ifndef _SUBSCRIBER_H_
#define _SUBSCRIBER_H_

#include "IObject.h"

class UntypedData;

class ick_api Subscriber : public IObject
{
public:

  Subscriber();

  virtual ~Subscriber();

public:

  virtual void update(UntypedData* untypedData) = 0;
};

#endif

