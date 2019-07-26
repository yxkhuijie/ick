#include "src/mysql_connection.h"
#include "src/logger.h"
#include "src/converter.h"

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
  this->chartset = "utf8";
}

MySqlConnection::~MySqlConnection() {}

void MySqlConnection::open() {
  sql::mysql::MySQL_Driver *driver;
  driver = sql::mysql::get_mysql_driver_instance();
  std::string server = "tcp://" + host + ":" + std::to_string(port);
  sql::ConnectOptionsMap option_map;
  option_map.insert(std::pair<sql::SQLString, sql::ConnectPropertyVal>("hostName", sql::Variant(sql::SQLString(server.c_str()))));
  option_map.insert(std::pair<sql::SQLString, sql::ConnectPropertyVal>("userName", sql::Variant(sql::SQLString(this->user.c_str()))));
  option_map.insert(std::pair<sql::SQLString, sql::ConnectPropertyVal>("password", sql::Variant(sql::SQLString(this->pwd.c_str()))));
  option_map.insert(std::pair<sql::SQLString, sql::ConnectPropertyVal>("OPT_RECONNECT", sql::Variant(true)));
  option_map.insert(std::pair<sql::SQLString, sql::ConnectPropertyVal>("OPT_CHARSET_NAME", sql::Variant(sql::SQLString(this->chartset.c_str()))));
  // connection_.reset(driver->connect(
  //     server, this->user, this->pwd));
  connection_.reset(driver->connect(option_map));
  connection_->setSchema(this->db);
  if (connection_ == nullptr) {
    Logger::getInstance()->Error("connection is null!!!");
  }
}

void MySqlConnection::close() {}

bool MySqlConnection::setCharset(std::string chartset) {
  this->chartset = chartset;
  return true;
}

bool MySqlConnection::isClosed() {
  return connection_->isClosed();
}

bool MySqlConnection::isValid() {
  return connection_->isValid();
}

void MySqlConnection::getConnection() {}

void MySqlConnection::releaseConnection(Connection* conn) {}

int MySqlConnection::executeQuery(const std::string& sql) {
  this->rs.clear();
  if(debug_) {
    Logger::getInstance()->Info("start execute query: sql: " + sql);
  }
  try {
    sql::Statement* stmt = connection_->createStatement();
    if (stmt == nullptr) {
      Logger::getInstance()->Error("stmt is null!!!");
    } else {
      result_set_.reset(stmt->executeQuery(sql));
      if (result_set_ != nullptr && result_set_->first()) {
        do {
          sql::ResultSetMetaData* meta_data = result_set_->getMetaData();
          unsigned int column_count = meta_data->getColumnCount();
          std::map<std::string, std::string> row_data;
          for (int i = 0; i < column_count; ++i) {
            std::string column_name = meta_data->getColumnName(i + 1);
            std::string column_value = result_set_->getString(i + 1);
            row_data.insert(std::pair<std::string, std::string>(column_name, column_value));
          }
          this->rs.push_back(row_data);
        } while (result_set_->next());
      }
      return result_set_->rowsCount();
    }
  } catch (const sql::SQLException& exception) {
    if (!debug_) {
      Logger::getInstance()->Info("executeQuery: sql: " + sql);
    }
    std::string msg = "executeQuery error: code: "
        + std::to_string(exception.getErrorCode())
        + ", what(): " + exception.what();
    Logger::getInstance()->Error(msg);
    return -1;
  }
  return 0;
}

bool MySqlConnection::executeCommand(const std::string& sql) {
  if(debug_) {
    Logger::getInstance()->Info("start execute command: sql: " + sql);
  }
  try {
    sql::Statement* stmt = connection_->createStatement();
    if (stmt == nullptr) {
      Logger::getInstance()->Error("stmt is null!!!");
      return false;
    } else {
      stmt->execute(sql);
    }
  } catch (const sql::SQLException& exception) {
    if (!debug_) {
      Logger::getInstance()->Info("executeCommand: sql: " + sql);
    }
    std::string msg = "executeCommand error: code: "
        + std::to_string(exception.getErrorCode())
        + ", what(): " + exception.what();
    Logger::getInstance()->Error(msg);
    return false;
  }
  return true;
}

RecordSet* MySqlConnection::getRecordSet() {
    return &this->rs;
}

sql::ResultSet* MySqlConnection::getResultSet() {
  return result_set_.get();
}
