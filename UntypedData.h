#ifndef _UNTYPED_DATA_H_
#define _UNTYPED_DATA_H_

#include "CmdTarget.h"
#include "ValueInfo.h"
#include "TypeInfo.h"
#include <list>
#include "Subscriber.h"
#include "IMutex.h"

#ifndef CHANNEL_MAX_LENGTH
#define CHANNEL_MAX_LENGTH 255
#endif

class ick_api UntypedData : public CmdTarget
{

public:

  UntypedData();

  UntypedData(int nodeNumber, int channelNumber);

  virtual ~UntypedData();

protected:

  /*
   * @ brief node number
   * @ comment: under one node, there may be more than one channel
   */
  int m_NodeNumber;

  /*
   * @ brief channel number
   * @ value: if node number == 0 and channel number == 0, 
   *          this channel will be set as logical channel, only used in software,
   *          else, the channel will be set as actual channel, communicated with hard ware. 
   */
  int m_ChannelNumber;

  /*
     * @ brief data type
     * @ value: UNKNOWN, INTDATA, DOUBLEDATA, STRINGDATA
     * @ value: default value is UNKNOWN
   */
  DataType m_dataType;

  /*
  * @ brief access mode
  * @ value: UNKNOWN, READONLY, READWRITE
  * @ value: default value is UNKNOWN
  */
  AccessMode m_accessMode;


  std::string m_strParamFirst;
  std::string m_strParamSecond;
  std::string m_strDescription;

  /*
  * @ brief  driver related to this data
  * @ value: default value is NULL
  */
  CmdTarget* m_driver;

  double m_accuracy;


  /*
  * @ brief  function register read related to this data
  * @ value: default value is NULL
  */
  FUNC_REG m_pfn_read;

  /*
  * @ brief  function register write related to this data
  * @ value: default value is NULL
  */
  FUNC_REG m_pfn_write;

  /*
  * @ brief  subscribers of this data, subclass of CmdTarget
  */
  std::list<Subscriber*> m_subscribers;

  bool m_isChanged;

  IMutex m_mutex;

  /*
  * @ brief  timestamp of this data, update when data value changed
  */
  long long m_timestamp;

  bool m_isValid;

public:
  
  /*
   * @ brief set node number
   * @ param nodeNumber: node number
   */
  void setNodeNumber(int nodeNumber);

  /*
  * @ brief get node number
  * @ return int: node number
  */
  int getNodeNumber();

  /*
  * @ brief set channel number
  * @ param channelNumber: channel number
  */
  void setChannelNumber(int channelNumber);

  void setFirstParamValue(std::string& value);
  void setSecondParamValue(std::string& value);
  void setDescription(std::string& value);

  std::string getFirstParamValue();
  std::string getSecondParamValue();
  std::string getDescription();

  /*
  * @ brief get channel number
  * @ return int: channel number
  */
  int getChannelNumber();

  /*
  * @ brief get value as string
  * @ return string: string format of value
  */
  virtual std::string getValueAsString() const = 0;

  /*
  * @ brief get value info
  * @ return ValueInfo: value info of data
  */
  // virtual ValueInfo getValueInfo() const = 0;

  /*
  * @ brief get type info
  * @ return TypeInfo: type info of data
  */
  // virtual TypeInfo getTypeInfo() const = 0;

  /*
  * @ brief get data type
  * @ return DataType: data type of data
  */
  DataType getDataType();

  /*
  * @ brief get access mode
  * @ return AccessMode: access mode of data
  */
  AccessMode getAccessMode();

  /*
  * @ brief subscribe from this data
  * @ comment: when value of this data changed, 
  *   object will be get notify through override function update of CmdTarget
  * @ param object: object of CmdTarget you want to notify
  */
  void subscribe(Subscriber* object);


  void unsubscribe(Subscriber* object);

  /*
  * @ brief register this data to driver
  * @ comment: if registed, pfn will be executed when you setValue or getValue
  * @ param pfn: function interface to regist
  * @ param dirver: object which pfn will be called 
  */
  virtual void registChannelRead(FUNC_REG pfn, CmdTarget* driver);

  /*
  * @ brief register this data to driver
  * @ comment: if registed, pfn will be executed when you setValue or getValue
  * @ param pfn: function interface to regist
  * @ param dirver: object which pfn will be called
  */
  virtual void registChannelWrite(FUNC_REG pfn, CmdTarget* driver);

  virtual void update();

  bool isChanged();
  
  bool isValid();
};

#endif

