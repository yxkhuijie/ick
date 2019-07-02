/********************************************************************
** Copyright (c) 2012, Beijing NMC Co.,Ltd.  All rights reserved.
** File name: ConnectionPool.h
** Description:  
** Release: 1.0
** Modification history:           
**           2016-06-16   yangxk create
** 
*********************************************************************/


#ifndef _CONNECTION_POOL_H_
#define _CONNECTION_POOL_H_

#include "src/control_object.h"
#include <list>
#include <map>
#include <iostream>
#include "src/connection.h"
#include "src/mysql_connection.h"
#include "src/mutex.h"

using namespace std;

typedef enum tagEDataBaseType
{
    MySql     = 0,
  Oracle    = 1,
  SqlServer = 2,
}EDataBaseType;

class ConnectionPoolManager;

class ick_api ConnectionPool : public ControlObject
{
    DECLARE_CLASS_DYNAMIC( ConnectionPool )
  DECLARE_MESSAGE_LIST

public:
  ConnectionPool(void);
  ~ConnectionPool(void);
  
  void make();
  void initialize();
  void verifyInit();
  void startup();
  void defaultAbort();

private:
  // 连接池属性  
  string driverName;  
  string url; 
  
  string rootPassword;
  string dbName; 
  string host;
  int port;
  string userName;  
  string password;
  string charset;
  string unixSocket;

    EDataBaseType dbType;  
  
  list<Connection*> freeConnections;
  list<Connection*> activeConnections;
  bool isActive;

    // 连接池名字  
    string poolName;  
    int minConnections;            // 空闲池，最小连接数  
    int maxConnections;            // 空闲池，最大连接数  
    int initConnections;           // 初始化连接数  
    int connTimeOut;               // 重复获得连接的频率    
    int maxActiveConnections;      // 最大允许的连接数，和数据库对应（数据库允许的最大连接数） 
    int connectionTimeOut;         // 连接超时时间，默认20分钟   
    bool m_isCurrentConnection;    // 是否获得当前连接，默认true   
    bool m_isCheckPool;            // 是否定时检查连接池  
    int lazyCheck;                 // 延迟多少时间后开始 检查  
    int periodCheck;               // 检查频率
  
  int countActive;               // 记录创建的总的连接数
    IMutex m_mutex;
public:
  void setDriverName(const string& driverName);
  void setUrl(const string& url);
  void setDataBaseType(const string& dbType);
  void setRootPassword(const string& password);
  void setDbName(const string& dbName);
  void setHost(const string& host);
  void setPort(int port);
  void setUserName(const string& userName);
  void setPassword(const string& password);
  void setCharset(const string& charset);
  void setUnixSocket(const string& unixSocket);
  void setPoolName(const string& poolName);
  void setMinConnections(int minConnections);
  void setMaxConnections(int maxConnections);
  void setMaxActiveConnections(int maxActiveConnections);
  void setInitConnections(int initConnections) ;
  void setConnTimeOut(int connTimeOut);
  void setConnectionTimeOut(int connectionTimeOut);
  void setCurrentConnection(bool isCurrentConnection);
  void setLazyCheckTime(int lazyCheck);
  void setPeriodCheckTime(int periodCheck) ;
  void setPoolCheck(bool isCheckPool);
  
  string getDriverName();
  string getUrl();
  string getRootPassword();
  string getDbName();
  int getPort();
  string getHost();
    string getUserName();
  string getPassword();
  string getCharset();
  string getPoolName();
  string getUnixSocket();
  int getMinConnections();
  int getMaxConnections();
  int getInitConnections();
  int getMaxActiveConnections();
  long getConnTimeOut();
  long getConnectionTimeOut();
  bool isCurrentConnection();
  long getLazyCheckTime();
  long getPeriodCheckTime();
  bool isCheckPool();
  
  bool checkTableExist(string tableName);
public:
    Connection* getConnection(void);
  void releaseConnection(Connection* conn);
  Connection* newConnection(void);
  bool isValid(Connection* conn);
};

#endif

