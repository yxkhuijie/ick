/********************************************************************
** Copyright (c) 2012, Beijing NMC Co.,Ltd.  All rights reserved.
** File name: Connection.h
** Description:  
** Release: 1.0
** Modification history:           
**           2016-06-16   yangxk create
** 
*********************************************************************/


#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "IObject.h"

#include <string>
#include <list>
#include <map>

#include "third_party/mysql_client_cpp/include/mysql_connection.h"
#include "third_party/mysql_client_cpp/include/mysql_driver.h"
#include "third_party/mysql_client_cpp/include/cppconn/resultset.h"
#include "third_party/mysql_client_cpp/include/cppconn/statement.h"

using namespace std;

typedef list<map<string, string> > RecordSet;

class ick_api Connection : public IObject
{
public:
  Connection(void){};
  virtual ~Connection(void){};
  
private: 
  int res;

public:
  RecordSet rs;
  std::unique_ptr<sql::ResultSet> result_set_;

public:
  virtual void open() = 0;
  virtual void close() = 0;
  virtual bool isClosed() = 0;
  virtual void getConnection() = 0;
  virtual void releaseConnection(Connection* conn) = 0;
  virtual int executeQuery(string& sql, bool print=false) = 0;
  virtual RecordSet* getRecordSet() = 0;
  virtual bool setCharset(std::string charset) { return true;  };
};

#endif

