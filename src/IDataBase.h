/********************************************************************
** Copyright (c) 2012, Beijing NMC Co.,Ltd.  All rights reserved.
** File name: IDataBase.h
** Description:  
** Release: 1.0
** Modification history:           
**           2016-06-21   yangxk create
** 
*********************************************************************/

#ifndef _DATABASE_UTIL_H_
#define _DATABASE_UTIL_H_

#include <string>
#include <list>
#include <map>
#include <iostream>
#include "ControlObject.h"
#include "UntypedData.h"
#include "Service.h"
#include "ConnectionPool.h"

using namespace std;

// typedef list<map<string,string>> RecordSet;

class ick_api IDataBase : public ControlObject
{
  DECLARE_CLASS_DYNAMIC(IDataBase)  
  DECLARE_MESSAGE_LIST
  DECLARE_SERVICEINFO_LIST
  
public:
  IDataBase(void);
  virtual ~IDataBase(void);

private:
  void make();
  void initialize();
  void verifyInit();
  void startup();
  
public:
  // ServicePtr<ExecuteCommand> m_ExecuteCommand;

  string sql;
  RecordSet rs;
  sql::ResultSet result_set_;
  int ErrCode;
  
  ConnectionPool* m_ConnectionPool;
  
  int executeQuery(std::string sql = "", bool print=false);
  int executeCommand(std::string sql = "");
  void clear(RecordSet& rs, bool freeMemory = true);

  SET_DEFINE(setConnectionPool)
};

#endif

