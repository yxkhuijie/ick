/********************************************************************
** Copyright (c) 2012, Beijing NMC Co.,Ltd.  All rights reserved.
** File name: MySqlConnection.h
** Description:  
** Release: 1.0
** Modification history:           
**           2016-07-11   yangxk create
** 
*********************************************************************/
#ifndef _MYSQL_CONNECTION_H_
#define _MYSQL_CONNECTION_H_

#include <string>
#include <map>
#include "AbstractException.h"
using namespace std;


class MySqlException : public AbstractException
{
public:
    MySqlException();
  ~MySqlException();
  
public:
  virtual void post(int errCode);
};

#endif
