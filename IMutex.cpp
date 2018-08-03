#include "IMutex.h"


#ifdef __windows__
IMutex::IMutex()
{
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);
	DWORD err = GetLastError();
	if (m_mutex == 0)
	{
		if (err == ERROR_ALREADY_EXISTS)
		{
			::CloseHandle(m_mutex);
			m_mutex = NULL;
		}
		throw "create mutex failure!!!";
	}
	else
	{
		if (err == ERROR_ALREADY_EXISTS)
		{
			::CloseHandle(m_mutex);
			m_mutex = NULL;
			throw "create mutex failure!!!";
		}
	}
}


IMutex::~IMutex()
{
	::CloseHandle(m_mutex);
}

void IMutex::lock()
{
	/*m_mutex.lock();*/
	//µ»∫Úª•≥‚  
	::WaitForSingleObject(m_mutex, INFINITE);
}

void IMutex::unlock()
{
	/*m_mutex.unlock();*/
	// Õ∑≈ª•≥‚  
	::ReleaseMutex(m_mutex);
}
#elif __linux__
IMutex::IMutex()
{
}


IMutex::~IMutex()
{
}

void IMutex::lock()
{
}

void IMutex::unlock()
{
}
#endif
