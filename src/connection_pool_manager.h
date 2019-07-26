/********************************************************************
** Copyright (c) 2012, Beijing NMC Co.,Ltd.  All rights reserved.
** File name: ConnectionPoolManager.h
** Description:  
** Release: 1.0
** Modification history:           
**           2016-06-18   yangxk create
** 
*********************************************************************/


#ifndef _CONNECTION_POOL_MANAGER_H_
#define _CONNECTION_POOL_MANAGER_H_

#include <map>
#include "src/connection_pool.h"
#include "src/connection.h"

using namespace std;

class ick_api ConnectionPoolManager {
 public:
  ConnectionPoolManager(void);
  ~ConnectionPoolManager(void);

 private:
  static ConnectionPoolManager* instance;  
  ConnectionPool* default_pool_;

 public:
  map<string, ConnectionPool*> pools;
  static ConnectionPoolManager* getInstance();
  void startup();
  Connection* getConnection(const string& poolName);
  void close(string poolName,Connection* conn);
  void destroy(string poolName);
  ConnectionPool* getPool(const string& poolName);
  void addPool(string poolName, ConnectionPool* pool);
};

#endif

