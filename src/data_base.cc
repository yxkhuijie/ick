#include "src/data_base.h"

#include "src/connection_pool_manager.h"
#include "src/object_manager.h"

IDataBase::IDataBase(void) {
  this->connection_pool_ = NULL;
}

IDataBase::~IDataBase(void) {
  this->clear(rs);
}

void IDataBase::make() {
  if(this->connection_pool_ == NULL) {
    throw getFullName()+string(" : Connection pool not set!");
  }
}

void IDataBase::initialize() {
  ControlObject::initialize();
}

void IDataBase::verifyInit() {}

void IDataBase::startup() {
  if(this->connection_pool_ == NULL) {
    Logger::getInstance()->Error("connection pool not set!!");
    std::string msg = "connection pool not set!!";
    throw msg;
  }
}

int IDataBase::executeQuery(std::string sql) {
  if (sql == "") sql = this->sql;
  if (connection_pool_ == nullptr) {
    connection_pool_ = ConnectionPoolManager::getInstance()->getPool(connection_pool_name_);
    if (connection_pool_ == nullptr) {
      Logger::getInstance()->Error("get connection pool error: pool name: " + connection_pool_name_);
    }
  }
  Connection* conn = this->connection_pool_->getConnection();
  if (conn != nullptr) {
    int ret = conn->executeQuery(sql);
    RecordSet* res = conn->getRecordSet();
    this->rs = *res;
    this->connection_pool_->releaseConnection(conn);
    return ret;
  } else {
    Logger::getInstance()->Error("get connection from pool failure: pool name: " + connection_pool_->getPoolName());
  }
  return -1;
}

bool IDataBase::executeCommand(std::string sql) {
  if(sql == "") sql = this->sql;
  Connection* conn = this->connection_pool_->getConnection();
  if(conn == NULL) {
    cout<<"IDataBase::GetDataMap, Get connection from pool failure." << endl;
    return -1;
  }
  bool flag = conn->executeCommand(sql);
  this->connection_pool_->releaseConnection(conn);
  return flag;
}

void IDataBase::clear(RecordSet& rs, bool freeMemory) {
  if(freeMemory) {
    RecordSet::iterator it;
    for(it = rs.begin(); it!=rs.end();++it) {
      // if(*it != NULL) delete (*it);
    }
  }
  rs.clear();
}

void IDataBase::setConnectionPoolName(const std::string& pool_name) {
  this->connection_pool_name_ = pool_name;
}

IMPLEMENT_CLASS_DYNAMIC(IDataBase, ControlObject)

SET(IDataBase,setConnectionPool,connection_pool_,ConnectionPool *)

BEGIN_SERVICEINFO_LIST(IDataBase)
  
END_SERVICEINFO_LIST(IDataBase)

IMPLEMENT_MESSAGE_LIST(IDataBase, ControlObject)

BEGIN_MESSAGE_LIST( IDataBase )
  SET_S(setConnectionPool, IDataBase)
  SET_S(setConnectionPoolName, IDataBase)
END_MESSAGE_LIST
