#include "IDataBase.h"
#include "ObjectManager.h"

IDataBase::IDataBase(void)
{
	this->m_ConnectionPool = NULL;
}


IDataBase::~IDataBase(void)
{
	this->clear(rs);
}

void IDataBase::make()
{
	if(this->m_ConnectionPool == NULL)
	{
		throw getFullName()+string(" : Connection pool not set!");
	}
}

void IDataBase::initialize()
{
	ControlObject::initialize();
}

void IDataBase::verifyInit()
{

}

void IDataBase::startup()
{
	
	//sql = "select * from T_COUNTER";
	//executeQuery();
	if(this->m_ConnectionPool == NULL)
	{
	    throw "\nConnectionPool has not been set, please check file database.xml at the node /Control/Counter/" + getSelfName();
	}
}

int IDataBase::executeQuery(std::string sql, bool print)
{
	if(sql == "") sql = this->sql;
	Connection* conn = this->m_ConnectionPool->getConnection();
	if(conn == NULL)
	{
	    cout<<"IDataBase::executeQuery, Get connection from pool failure. ConnectionPoolName: "<< this->m_ConnectionPool->getPoolName() << endl;
		return -1;
	}

	int ret = conn->executeQuery(sql);
	RecordSet* res = conn->getRecordSet();
	this->rs = *res;
	// cout<<"RecordSet* res: "<<(unsigned int)res<<endl;
	//if(res!=NULL)
	//{
	//       list<map<string,string> >::iterator it;
	//	for(it=res->begin();it!=res->end();++it)
	//	{
	//	    map<string, string>::iterator itor;
	//		// map<string, string> m = *it;
	//		/*for(itor=it->begin();itor!=it->end();++itor)
	//		{
	//		    m.insert(pair<string,string>(itor->first, itor->second));
	//		}*/
	//		this->rs.push_back(*it);
	//	}
	//}
	this->m_ConnectionPool->releaseConnection(conn);
	return ret;
}

int IDataBase::executeCommand(std::string sql)
{
	if(sql == "") sql = this->sql;
    Connection* conn = this->m_ConnectionPool->getConnection();

	if(conn == NULL)
	{
	    cout<<"IDataBase::GetDataMap, Get connection from pool failure." << endl;
		return -1;
	}

	int flag = conn->executeQuery(sql);
	this->m_ConnectionPool->releaseConnection(conn);
	return flag;
}

void IDataBase::clear(RecordSet& rs, bool freeMemory)
{
	if(freeMemory)
	{
		RecordSet::iterator it;
		for(it = rs.begin(); it!=rs.end();++it)
		{
			// if(*it != NULL) delete (*it);
		}
	}
	rs.clear();
}

IMPLEMENT_CLASS_DYNAMIC(IDataBase, ControlObject)

SET(IDataBase,setConnectionPool,m_ConnectionPool,ConnectionPool *)

BEGIN_SERVICEINFO_LIST(IDataBase)
	
END_SERVICEINFO_LIST(IDataBase)

IMPLEMENT_MESSAGE_LIST(IDataBase, ControlObject)

BEGIN_MESSAGE_LIST( IDataBase )
	SET_S(setConnectionPool, IDataBase)
END_MESSAGE_LIST
