#include "MySqlConnection.h"
#include "Logger.h"
#include "Converter.h"

MySqlConnection::MySqlConnection(string host,string user,string pwd, string db, int port, string unix_socket, unsigned int client_flag)
{
    // this->pMySql      = NULL;
	mysql_init(&this->mysql);
	cout << "Constructor MySqlConnection: pMySql: " << (unsigned int)&this->mysql << endl;
	
	this->host        = host;
	this->user        = user;
	this->pwd         = pwd;
	this->db          = db;
	this->port        = port;
	this->unix_socket = unix_socket;
	this->client_flag = client_flag;
}

MySqlConnection::~MySqlConnection()
{

}

void MySqlConnection::open()
{
    MYSQL *pMySql = mysql_real_connect(&this->mysql, host.c_str(), user.c_str(), pwd.c_str(), db.c_str(), port, unix_socket.c_str(), client_flag);
	if(pMySql == NULL)
	{
	    // mysql connect failure
		cout << "mysql connect failure, error in MySqlConnection::open"<<endl;
		//string str = mysql_error(this->pMySql);
		cout << "pMySql: " << (unsigned int)pMySql << ", mysql: " << (unsigned int)&mysql << endl;
		cout << "error code: " << mysql_errno(&mysql) << ", error string: " << mysql_error(&mysql) << endl;  
		cout << "host:" + host + ", port:" << port << + ", user:" + user + ", pwd:" + pwd + ", unix_socket:" + unix_socket<<endl; 
	}
	else
	{
	    char val = 1;
	    // mysql_options(&mysql, MYSQL_OPT_RECONNECT,(char*)&val);      // 设置数据库连接自动重连(MySql 5.1 以下版本不支持)
	    cout << "mysql connect success, "<<"mysql: " << (unsigned int)&mysql << endl;
	}
}

void MySqlConnection::close()
{
	mysql_close(&mysql);
    cout << "Destructor MySqlConnection: mysql: " << (unsigned int)&mysql << endl;
}

bool MySqlConnection::setCharset(std::string chartset)
{
	std::string str = std::string("set names '") + chartset + std::string("'");
	if (mysql_query(&this->mysql, str.c_str()) < 0)
	{
		printf("mysql connection set chartset failure: chartset: %s", chartset.c_str());
		return false;
	}
	return true;
}

bool MySqlConnection::isClosed()
{
	int flag = mysql_ping(&mysql);
	if(flag == 0) return false;
	else
	{
	    cout << "mysql connection has been closed: error code: " << flag << endl;
	    return true;
	}
}

void MySqlConnection::getConnection()
{

}


void MySqlConnection::releaseConnection(Connection* conn)
{

}


int MySqlConnection::executeQuery(string& sql, bool print)
{
    this->rs.clear();
    int flag = mysql_real_query(&mysql, sql.c_str(), sql.length());
	if(print) cout<<"start executeQuery: sql: " + sql << endl;
	int ret = -1;
    if(flag == 0)
    {
	    MYSQL_RES *result=mysql_store_result(&mysql);    
		if(result==NULL)
		{
			if (print) cout<<"MySqlConnection::executeQuery result is null\n";
			return 0;
		}
		int rowcount=mysql_num_rows(result);                //获取记录数
        unsigned int fieldcount=mysql_num_fields(result);   //获取字段数
		MYSQL_FIELD *field=NULL;                            //字段
        MYSQL_ROW row=NULL;                                 //记录
		
		for (unsigned int i = 0; i < fieldcount; i++)
		{
			field = mysql_fetch_field_direct(result, i);
			if (print)
			{
				if (i == fieldcount - 1) cout << field->name << endl;
				else cout << field->name << "  |  ";
			}
		}
		
        while(row=mysql_fetch_row(result))
        {
		    map<string,string> m; // = new map<string, string>();
            for(unsigned int i=0;i<fieldcount;i++)
            {
				string str = row[i] == NULL ? "" : row[i];
				field=mysql_fetch_field_direct(result,i);
				
		        m.insert(pair<string,string>(field->name,str));
				if (print)
				{
					if (i == fieldcount - 1) cout << str << endl;
					else cout << str << "  |  ";
				}
            }
			this->rs.push_back(m);
        }  
        mysql_free_result(result);
		ret = rowcount;
	}
	else
	{
		Logger::getInstance()->Error(std::string("executeQuery error: ret: -1, mysql error id: ") + Converter::convertToString(flag) + std::string(", sql: ") + sql);
		return ret;
	}
	if (ret < 0) Logger::getInstance()->Error(std::string("executeQuery error: sql: ") + sql);
	if(print || ret < 0) cout<<"end executeQuery: ret: " << ret << endl;
	
	return ret;
}

RecordSet* MySqlConnection::getRecordSet()
{
    return &this->rs;
}


