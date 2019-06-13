#ifndef _ISERIAL_PORT_H_
#define _ISERIAL_PORT_H_

#include "IObject.h"


class ick_api ISerialPort : public IObject
{

public:

  ISerialPort();

  virtual ~ISerialPort();

protected:
  int inputQueneLength;

public:

  // Open the serial communications for a particular COM port. You
  // need to use the full devicename (i.e. "COM1") to open the port.
  // It's possible to specify the size of the input/output queues.
  long open(std::string strDevice, unsigned long queneInput, unsigned long queneOutput);

public:

  

};

#endif
