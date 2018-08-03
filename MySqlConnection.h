/********************************************************************
** Copyright (c) 2012, Beijing NMC Co.,Ltd.  All rights reserved.
** File name: MySqlConnection.h
** Description:  
** Release: 1.0
** Modification history:           
** 					2016-06-16   yangxk create
** 
*********************************************************************/
#ifndef _MYSQL_CONNECTION_H_
#define _MYSQL_CONNECTION_H_

#ifdef _WIN64
#include <WinSock2.h>
#elif _WIN32
#include <winsock.h>
#endif

#include "Connection.h"
#include "mysql.h"
#include <string>
#include <iostream>

using namespace std;

class ick_api MySqlConnection : public Connection
{
public:
    MySqlConnection(string host,string username,string pwd, string db, int port = 3306, string unix_socket = "", unsigned int client_flag = 0);
	virtual ~MySqlConnection();

private:
	MYSQL mysql;
	
	string host;
	string user;
	string pwd;
	string db;
	int    port;
	string unix_socket;
	unsigned int client_flag;

public:
    virtual void open();
	virtual void close();
	virtual bool isClosed();
    virtual void getConnection();
	virtual void releaseConnection(Connection* conn);
	virtual int executeQuery(string& sql, bool print=false);              //  执行成功返回受影响的行数
	virtual RecordSet* getRecordSet();
	virtual bool setCharset(std::string chartset);
};

#endif
