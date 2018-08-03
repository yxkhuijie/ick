#ifndef _IMUTEX_H_
#define _IMUTEX_H_

#include "IObject.h"
//#include <Ice/Ice.h>

#ifdef WIN32
#include <Windows.h>
#endif

class ick_api IMutex : public IObject
{

public:
	
	IMutex();
	
	virtual ~IMutex();

private:

	/*IceUtil::Mutex m_mutex;*/
	HANDLE m_mutex;
public:

	void lock();

	void unlock();
};

#endif
