#ifndef _IDRIVER_H_
#define _IDRIVER_H_

#include "ControlObject.h"
#include "IProtocol.h"
#include "ObjectManager.h"

class ick_api IDriver : public ControlObject
{
  DECLARE_CLASS_DYNAMIC(IDriver)
  DECLARE_MESSAGE_LIST

public:

  IDriver();
  virtual ~IDriver();

protected:

  /*
   * @ brief node number, used when channel regist to this driver
   * @ comment: default auto regist channels which node number(Bd) is same with this value
   * @ comment: if you want to regist other channels which node number(Bd) not same with this value,
   * @ comment: you can override the interface initChannels.
   */
  int        m_nodeNumber;

  /*
  * @ protocl used in this driver
  */
  IProtocol* m_protocol;

  /*
   * @ brief retry times when request data failed
     * @ value default value is 3
   */
  int        m_retryTimes;

public:

  void setRetryTimes(int num);

  void setNodeNumber(int nodeNumber);

  /*
   * @ brief register channels
  */
  virtual void registChannels();

  virtual bool registReadInt(int channelNumber, int* value);

  virtual bool registReadDouble(int channelNumber, double* value);

  virtual bool registReadString(int channelNumber, std::string& value);

  virtual bool registWriteInt(int channelNumber, int value);

  virtual bool registWriteDouble(int channelNumber, double value);

  virtual bool registWriteString(int channelNumber, const std::string& value);

  SET_DEFINE(setProtocol)
};

#endif


