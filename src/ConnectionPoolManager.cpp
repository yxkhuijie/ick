#include "ConnectionPoolManager.h"

ConnectionPoolManager* ConnectionPoolManager::instance = new ConnectionPoolManager();

ConnectionPoolManager::ConnectionPoolManager(void)
{
  startup();
}


ConnectionPoolManager::~ConnectionPoolManager(void)
{
    
}

// 单例实现  
ConnectionPoolManager* ConnectionPoolManager::getInstance()
{  
    return instance;  
}  

// 初始化所有的连接池  
void ConnectionPoolManager::startup()
{  
  
}  

Connection* ConnectionPoolManager::getConnection(string poolName)
{
    Connection* conn = NULL;  
  if(pools.size()>0 && (pools.find(poolName) != pools.end()))
    {  
        conn = this->getPool(poolName)->getConnection();  
    }
  else
    {  
        // System.out.println("Error:Can't find this connecion pool ->"+poolName);  
    }  
    return conn;  
}

// 关闭，回收连接  
void ConnectionPoolManager::close(string poolName,Connection* conn)
{  
    ConnectionPool* pool = getPool(poolName);  
 
    if(pool != NULL)
    {  
        //pool.releaseConn(conn);  
    }  

    //catch (SQLException e) 
    //{  
    //    System.out.println("连接池已经销毁");  
    //    e.printStackTrace();  
    //}  
}  
      
// 清空连接池  
void ConnectionPoolManager::destroy(string poolName)
{  
    ConnectionPool* pool = getPool(poolName);  
    if(pool != NULL)
    {  
        // pool.destroy();  
    }  
}  
      
// 获得连接池
ConnectionPool* ConnectionPoolManager::getPool(string poolName)
{  
    ConnectionPool* pool = NULL;  
    if(pools.size() > 0)
    {  
      map<string,ConnectionPool*>::iterator it = pools.find(poolName);
    if(it != pools.end())
    {
        pool = it->second;
    }
    }  
    return pool;  
}

void ConnectionPoolManager::addPool(string poolName, ConnectionPool* pool)
{
    if(pools.find(poolName) == pools.end()) 
  {
      pools[poolName] = pool;
    //throw "Connection pool named '" + poolName  + "' added success, occured in ConnectionPoolManager::addPool";
  }
  else
  {
      throw "\nConnection pool named '" + poolName  + "' has been existed, error in ConnectionPoolManager::addPool().";
  }
}


