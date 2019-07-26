#include "src/connection_pool_manager.h"

ConnectionPoolManager* ConnectionPoolManager::instance = new ConnectionPoolManager();

ConnectionPoolManager::ConnectionPoolManager(void)
    : default_pool_(nullptr) {
  startup();
}


ConnectionPoolManager::~ConnectionPoolManager(void) {}
 
ConnectionPoolManager* ConnectionPoolManager::getInstance() {
  return instance;
}  
 
void ConnectionPoolManager::startup() {}

Connection* ConnectionPoolManager::getConnection(const string& poolName) {
  ConnectionPool* pool = getPool(poolName);
  return pool ? pool->getConnection() : nullptr;
}

void ConnectionPoolManager::close(string poolName,Connection* conn) { 
  ConnectionPool* pool = getPool(poolName);
  if(pool != NULL) {}  
}  
      
void ConnectionPoolManager::destroy(string poolName) { 
  ConnectionPool* pool = getPool(poolName);  
  if(pool != NULL) {}  
}  

ConnectionPool* ConnectionPoolManager::getPool(const string& poolName) {
  ConnectionPool* pool = NULL;
  if (poolName.empty()) {
    if (default_pool_) return default_pool_;
    if (pools.size() == 1) {
      return pools.begin()->second;
    }
  } else if(!poolName.empty() && pools.size() > 0) {
    map<string,ConnectionPool*>::iterator it = pools.find(poolName);
    if(it != pools.end()) {
      pool = it->second;
    }
  }
  return pool;  
}

void ConnectionPoolManager::addPool(string poolName, ConnectionPool* pool) {
  if(pools.find(poolName) == pools.end()) {
    pools[poolName] = pool;
  } else {
    throw "\nConnection pool named '" + poolName  + "' has been existed, error in ConnectionPoolManager::addPool().";
  }
}


