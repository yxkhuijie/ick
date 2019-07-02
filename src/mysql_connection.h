#ifndef _SEC_MYSQL_CONNECTION_H_
#define _SEC_MYSQL_CONNECTION_H_

#ifdef _WIN64
#include <WinSock2.h>
#elif _WIN32
#include <winsock.h>
#endif


#include "src/connection.h"
#include <string>
#include <iostream>

#include "third_party/mysql_client_cpp/include/mysql_connection.h"

using namespace std;

class ick_api MySqlConnection : public Connection {
 public:
  MySqlConnection(
      std::string host,
      std::string username,
      std::string pwd,
      std::string db,
      int port = 3306,
      string unix_socket = "",
      unsigned int client_flag = 0);
  virtual ~MySqlConnection();

 private:
  string host;
  string user;
  string pwd;
  string db;
  int port;
  string unix_socket;
  unsigned int client_flag;
  std::unique_ptr<sql::Connection> connection_;

 public:
  virtual void open();
  virtual void close();
  virtual bool isClosed();
  virtual bool isValid();
  virtual void getConnection();
  virtual void releaseConnection(Connection* conn);
  virtual int executeQuery(string& sql, bool print=false);
  virtual bool executeCommand(string& sql, bool print=false);
  virtual RecordSet* getRecordSet();
  virtual sql::ResultSet* getResultSet();
  virtual bool setCharset(std::string chartset);
};

#endif
