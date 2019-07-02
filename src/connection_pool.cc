#include "src/connection_pool.h"
#include "src/connection_pool_manager.h"
#include "src/converter.h"

ConnectionPool::ConnectionPool(void)
{
    countActive = 0;
  rootPassword = "";
  charset = "utf8";
}

ConnectionPool::~ConnectionPool(void) {
    for(list<Connection*>::iterator it = this->freeConnections.begin();it != this->freeConnections.end();) {
        if(*it != NULL) {
            (*it) -> close();
            delete (*it);
            it = freeConnections.erase(it);
            Logger::getInstance()->Info("DataBase connection close success");
        }
    }
}

void ConnectionPool::make()
{
  // DISPLAY(": start make()")  
  // 初始化服务
  // SERVICE_MAKE(Init)
  
  for(int i=0;i<this->initConnections;i++)
  {
      Connection* conn = this->newConnection();
    if(conn == NULL)
    {
      std::string msg = "DataBase connection initialize failure, error in ConnectionPool::initialize.";
      Logger::getInstance()->Error(msg);
    }
    else
    {
      conn->open();
      conn->setCharset(this->charset);
      if(!this->isValid(conn)) {
        std::string msg = std::string("DataBase connection is not valid, conn: ");
        Logger::getInstance()->Error(msg);
        throw std::string("DataBase connection is not valid, conn: ");
      }
      freeConnections.push_back(conn);
    }
  }
  
  // 将连接池加入连接池管理器
  ConnectionPoolManager::getInstance()->addPool(this->poolName, this);

    ControlObject::make();
    // DISPLAY(": finished make()")
}

void ConnectionPool::initialize()
{
    // DISPLAY(": start init()")
  
  //for(int i=0;i<this->initConnections;i++)
  //{
  //    Connection* conn = this->newConnection();
  //  if(conn == NULL)
  //  {
  //    std::string msg = "DataBase connection initialize failure, error in ConnectionPool::initialize.";
  //    Logger::getInstance()->Error(msg);
  //  }
  //  else
  //  {
  //    std::string msg = std::string("DataBase connection initialize success, conn: ") + Converter::convertToString((int)conn);
  //    Logger::getInstance()->Info(msg);

  //    conn->open();
  //    conn->setCharset(this->charset);
  //    if(!this->isValid(conn)) 
  //    {
  //      msg = std::string("DataBase connection is not valid, conn: ") + Converter::convertToString((int)conn);
  //      Logger::getInstance()->Error(msg);
  //        throw std::string("DataBase connection is not valid, conn: ") + Converter::convertToString((int)conn);
  //    }
  //    freeConnections.push_back(conn);
  //  }
  //}
  //
  //// 将连接池加入连接池管理器
  //ConnectionPoolManager::getInstance()->addPool(this->poolName, this);
  
    ControlObject::initialize();
    // DISPLAY(": finished init()")  
}

void ConnectionPool::verifyInit()
{
    //DISPLAY(": start verifyInit()")    
    //SysLogger::getInstance()->logMsg(LOGBRANCH::APP, LEVEL::EVENT, string("Starting verifyInit() for ConnectionPool"));
    //ControlObject::verifyInit();    
    //SysLogger::getInstance()->logMsg(LOGBRANCH::APP, LEVEL::EVENT, string("Finished verifyInit() for ConnectionPool"));
    //DISPLAY(": finished verifyInit()")  
}

void ConnectionPool::startup()
{
    //DISPLAY(": start startup()")
    //DISPLAY(": finished startup()")
}

void ConnectionPool::defaultAbort()
{   
  //DISPLAY(": start ConnectionPool::defaultAbort()")
  //SysLogger::getInstance()->logMsg(LOGBRANCH::APP, LEVEL::EVENT, getName() + string(" start defaultAbort()"));
  //m_serviceSts->setValue("Aborted");
  //ControlObject::defaultAbort();    
  //SysLogger::getInstance()->logMsg(LOGBRANCH::APP, LEVEL::EVENT, getName() + string(" Finished defaultAbort()"));
  //DISPLAY(": finished ConnectionPool::defaultAbort()")
}  

Connection* ConnectionPool::getConnection(void)
{
    // 此处应该加锁
  this->m_mutex.lock();
    Connection* conn = NULL;
  if(countActive < this->maxActiveConnections)
  {
      if(freeConnections.size() > 0)
    {
        conn = *(freeConnections.begin());
      if(conn != NULL)
      {
          
      }
      freeConnections.pop_front();
    }
    else
    {
        conn = newConnection();
      if (conn != NULL) conn->open();
      if (conn != NULL) conn->setCharset(this->charset);
    }
  }
  else
  {
      cout<<"reget connection, sleep time: " << this->connTimeOut << "countActive: " << countActive << endl;
      this->m_mutex.unlock();
    msleep(this->connTimeOut);
    conn = getConnection();
  }
  
  if (isValid(conn)) 
    {
      activeConnections.push_back(conn);  
        countActive++;  
  }
  this->m_mutex.unlock();
  return conn;
}

void ConnectionPool::releaseConnection(Connection* conn)
{
    list<Connection*>::iterator itor;
  for(itor=activeConnections.begin();itor!=activeConnections.end();++itor)
  {
      Connection* conn = *itor;
    if(conn!=NULL)
    {
        activeConnections.remove(*itor);
      freeConnections.push_back(conn);
      countActive--;
      break;
    }
  }
}

Connection* ConnectionPool::newConnection()
{
    Connection* conn = NULL;
  switch(this->dbType)
  {
  case MySql:
    conn = new MySqlConnection(host,userName,password, dbName, port, unixSocket);
    break;
  case Oracle:
    break;
  case SqlServer:
    break;
  default:
    break;
  }
  
  return conn;
}

bool ConnectionPool::isValid(Connection* conn)
{
    if(conn == NULL || conn->isClosed()) return false;
  return true;
}

string ConnectionPool::getDriverName() 
{  
    if(driverName == "")
    {  
        driverName = this->getDriverName()+"_"+this->getUrl();  
    }  
    return driverName;
}  

void ConnectionPool::setDriverName(const string& driverName) 
{  
    this->driverName = driverName;  
}  

void ConnectionPool::setDataBaseType(const string& dbType)
{  
    if(dbType == "MySql")
  {
      this->dbType = MySql;  
  }
}  

void ConnectionPool::setRootPassword(const string& password)
{
  this->rootPassword = password;
}

string ConnectionPool::getRootPassword()
{
  return this->rootPassword;
}

string ConnectionPool::getUrl() 
{  
    return url;  
} 
    
void ConnectionPool::setUrl(const string& url)
{  
    this->url = url;  
} 

string ConnectionPool::getDbName()
{
  return this->dbName;
}
void ConnectionPool::setDbName(const string& dbName)
{
  this->dbName = dbName;
}

string ConnectionPool::getHost()
{
  return this->host;
}
void ConnectionPool::setHost(const string& host)
{
  this->host = host;
}
int ConnectionPool::getPort()
{
  return this->port;
}
void ConnectionPool::setPort(int port)
{
  this->port = port;
}

string ConnectionPool::getUserName() 
{  
    return userName;  
}
    
void ConnectionPool::setUserName(const string& userName)
{  
    this->userName = userName;  
}
    
string ConnectionPool::getPassword() 
{  
    return password;  
} 

string ConnectionPool::getCharset()
{
  return charset;
}
    
void ConnectionPool::setPassword(const string& password)
{  
    this->password = password;  
}  

void ConnectionPool::setCharset(const string& charset)
{
  this->charset = charset;
}
   
void ConnectionPool::setUnixSocket(const string& unixSocket)
{
    this->unixSocket = unixSocket;
}
string ConnectionPool::getUnixSocket()
{
    return this->unixSocket;
}
   
string ConnectionPool::getPoolName()
{  
    return poolName;  
} 
    
void ConnectionPool::setPoolName(const string& poolName)
{  
    this->poolName = poolName;  
} 
    
int ConnectionPool::getMinConnections() 
{  
    return minConnections;  
} 
    
void ConnectionPool::setMinConnections(int minConnections) 
{  
    this->minConnections = minConnections;  
}
    
int ConnectionPool::getMaxConnections() 
{  
    return maxConnections;  
} 
    
void ConnectionPool::setMaxConnections(int maxConnections) 
{  
    this->maxConnections = maxConnections;  
} 
    
int ConnectionPool::getInitConnections() 
{  
    return initConnections;  
} 
    
void ConnectionPool::setInitConnections(int initConnections) 
{  
    this->initConnections = initConnections;  
}  
  
int ConnectionPool::getMaxActiveConnections() 
{  
    return maxActiveConnections;  
} 
    
void ConnectionPool::setMaxActiveConnections(int maxActiveConnections) 
{  
    this->maxActiveConnections = maxActiveConnections;  
}
    
long ConnectionPool::getConnTimeOut() 
{  
    return connTimeOut;  
} 
    
void ConnectionPool::setConnTimeOut(int connTimeOut) 
{  
    this->connTimeOut = connTimeOut;  
}
    
long ConnectionPool::getConnectionTimeOut() 
{  
    return connectionTimeOut;  
}
    
void ConnectionPool::setConnectionTimeOut(int connectionTimeOut)
{  
    this->connectionTimeOut = connectionTimeOut;  
}  
    
bool ConnectionPool::isCurrentConnection()
{  
    return m_isCurrentConnection;  
} 
    
void ConnectionPool::setCurrentConnection(bool isCurrentConnection) 
{  
    this->m_isCurrentConnection = isCurrentConnection;  
} 
    
long ConnectionPool::getLazyCheckTime() 
{  
    return lazyCheck;  
}  
void ConnectionPool::setLazyCheckTime(int lazyCheck) 
{  
    this->lazyCheck = lazyCheck;  
}
    
long ConnectionPool::getPeriodCheckTime() 
{  
    return periodCheck;  
} 
    
void ConnectionPool::setPeriodCheckTime(int periodCheck) 
{  
    this->periodCheck = periodCheck;  
}
    
bool ConnectionPool::isCheckPool() 
{  
    return m_isCheckPool;  
} 
    
void ConnectionPool::setPoolCheck(bool isCheckPool) 
{  
    this->m_isCheckPool = isCheckPool;  
}

bool ConnectionPool::checkTableExist(string tableName)
{
  return true;
}


IMPLEMENT_CLASS_DYNAMIC(ConnectionPool, ControlObject)

IMPLEMENT_MESSAGE_LIST(ConnectionPool, ControlObject)
BEGIN_MESSAGE_LIST( ConnectionPool )
  SET_S(setDriverName, ConnectionPool )  
    SET_S(setDataBaseType, ConnectionPool )  
  
  SET_S(setRootPassword, ConnectionPool )  
  
  SET_S(setHost, ConnectionPool )
  SET_S(setDbName, ConnectionPool )
  SET_I(setPort, ConnectionPool )
  SET_S(setUserName, ConnectionPool )
    
  SET_S(setPassword, ConnectionPool )
  SET_S(setCharset, ConnectionPool)
  SET_S(setUnixSocket, ConnectionPool )
  SET_S(setPoolName, ConnectionPool )
    
  SET_I(setMinConnections, ConnectionPool )
  SET_I(setMaxConnections, ConnectionPool )
  SET_I(setMaxActiveConnections, ConnectionPool )
  
  SET_I(setInitConnections, ConnectionPool )
  SET_I(setMaxActiveConnections, ConnectionPool )
  
  SET_I(setConnTimeOut, ConnectionPool )
  SET_I(setConnectionTimeOut, ConnectionPool )

  SET_B(setCurrentConnection, ConnectionPool )    
  SET_I(setLazyCheckTime, ConnectionPool )
  
  SET_I(setPeriodCheckTime, ConnectionPool )
  SET_B(setPoolCheck, ConnectionPool )
END_MESSAGE_LIST

