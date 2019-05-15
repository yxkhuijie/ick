/********************************************************************
** Copyright (c) 2012, Beijing NMC Co.,Ltd.  All rights reserved.
** File name: MySqlConnection.h
** Description:  
** Release: 1.0
** Modification history:           
**           2016-07-11   yangxk create
** 
*********************************************************************/
#ifndef _ABSTRACT_EXCEPTION_H_
#define _ABSTRACT_EXCEPTION_H_

#include <string>
#include <map>
using namespace std;

class AbstractException
{
public:
  AbstractException();
  virtual ~AbstractException();

public:
  class AlarmInfo
  {
    int m_almId;
    string m_errMsg;
  
  public:
    AlarmInfo(int almId, string errMsg){};
    ~AlarmInfo(){};
  };
  
public:
  int m_errCode;
  map<int,AlarmInfo> m_alarmInfoList;
   
  virtual void post(int errCode) = 0;
  virtual void AddAlarm(string& almName, int errCode, string almMsg);
  virtual void setAlarm(int errCode, int almId, string desp);
};

#endif
