#include "src/mysql_connection.h"
#include "Logger.h"
#include "Converter.h"

MySqlConnection::MySqlConnection(
    std::string host,  // default: tcp://127.0.0.1:3306
    std::string user,
    std::string pwd,
    std::string db,
    int port,
    std::string unix_socket,
    unsigned int client_flag) { 
  this->host = host;
  this->user = user;
  this->pwd = pwd;
  this->db = db;
  this->port = port;
  this->unix_socket = unix_socket;
  this->client_flag = client_flag;
}

MySqlConnection::~MySqlConnection() {}

void MySqlConnection::open() {
  sql::mysql::MySQL_Driver *driver;
  driver = sql::mysql::get_mysql_driver_instance();
  connection_.reset(driver->connect(
      "tcp://127.0.0.1:3306", this->user, this->pwd));
  if (connection_ == nullptr) {
    Logger::getInstance()->Error("connection is null!!!");
  }
}

void MySqlConnection::close() {}

bool MySqlConnection::setCharset(std::string chartset) {
  return true;
}

bool MySqlConnection::isClosed() {
  return true;
}

void MySqlConnection::getConnection() {

}


void MySqlConnection::releaseConnection(Connection* conn) {

}


int MySqlConnection::executeQuery(string& sql, bool print) {
  this->rs.clear();
  if(print) cout<<"start execute query: sql: " + sql << endl;
  sql::Statement* stmt = connection_->createStatement();
  if (stmt == nullptr) {
    Logger::getInstance()->Error("stmt is null!!!");
  } else {
    result_set_.reset(stmt->executeQuery(sql));
  }
  return 0;
}

RecordSet* MySqlConnection::getRecordSet() {
    return &this->rs;
}

sql::ResultSet* MySqlConnection::getResultSet() {
  return result_set_.get();
}


