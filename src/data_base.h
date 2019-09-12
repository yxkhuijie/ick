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
#include "src/control_object.h"
#include "src/untyped_data.h"
#include "src/service.h"
#include "src/connection_pool.h"

using namespace std;

// typedef list<map<string,string>> RecordSet;

class ick_api IDataBase : public ControlObject {
  DECLARE_CLASS_DYNAMIC(IDataBase)
  DECLARE_MESSAGE_LIST
  DECLARE_SERVICEINFO_LIST
  
public:
  IDataBase(void);
  virtual ~IDataBase(void);

private:
  void make();
  void initialize();
  void verify();
  void startup();
  
public:
  string sql;
  RecordSet rs;
  int ErrCode;

  void setConnectionPoolName(const std::string& pool_name);
  int executeQuery(std::string sql = "");
  bool executeCommand(std::string sql = "");
  void clear(RecordSet& rs, bool freeMemory = true);

  SET_DEFINE(setConnectionPool)

 private:
  ConnectionPool* connection_pool_;
  std::string connection_pool_name_;
};

#endif

