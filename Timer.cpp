#include "Timer.h"

int Timer::timerId = 1;

Timer::Timer()
{
	this->m_interval = 1000;
	this->m_parent = this;
	this->m_state = TIMER_STATE_IDLE;
#ifdef __windows__
	this->m_thread = NULL;
#elif __linux__
	this->m_thread = 0;
#endif
	this->m_exitFlag = false;
	this->m_timerId = timerId++;
}


Timer::~Timer()
{
	// ½áÊøÏß³Ì
	this->exit();
}

void Timer::execute()
{
	while(this->m_exitFlag == false)
	{
		if(this->m_parent != NULL)
		{
			m_parent->onTimer(this);
		}

		int t = this->m_interval;
		while( t > 0)
		{
#ifdef __windows__
			::Sleep(t > 10 ? 10 : t);
#elif __linux__
			::usleep(t > 10 ? 10000 : (t*1000));
#endif
			if(this->m_exitFlag == true)
			{
				this->m_state = TIMER_STATE_IDLE;
				return;
			}
			t -= 10;
		}
	}

	this->m_state = TIMER_STATE_IDLE;
}

void Timer::setParent(Timer* parent)
{
	this->m_parent = parent;
}

void Timer::setTimerId(int timerId)
{
	this->m_timerId = timerId;
}

int Timer::getTimerId()
{
	return this->m_timerId;
}


void Timer::setInterval(int millisecond)
{
	if(millisecond >= 0)
	{
		this->m_interval = millisecond;
	}
}

void Timer::start()
{
	this->m_mutex.lock();
#ifdef _WINDOWS
	
	if(this->m_state == TIMER_STATE_IDLE)
	{
		this->m_thread = CreateThread(NULL, 0, TimerHandler, this, 0, NULL);
		if(this->m_thread != NULL)
		{
			this->m_state = TIMER_STATE_RUNNING;
		}
		else
		{
			this->m_state = TIMER_STATE_IDLE;
		}
	}
	else if(this->m_state == TIMER_STATE_STOPPED && this->m_thread != NULL)
	{
		::ResumeThread(this->m_thread);
	}
#endif
	this->m_mutex.unlock();
}

void Timer::stop()
{
#ifdef __windows__
	this->m_mutex.lock();
	if(this->m_state == TIMER_STATE_RUNNING && this->m_thread != NULL)
	{
		if(::SuspendThread(this->m_thread) != 0xFFFFFFFF)
		{
			this->m_state = TIMER_STATE_STOPPED;
		}

	}
#elif __linux__
#endif
	this->m_mutex.unlock();
}

void Timer::exit()
{
	this->m_mutex.lock();
	if(this->m_state == TIMER_STATE_IDLE) 
	{
		this->m_mutex.unlock();
		return;
	}
	
#ifdef __windows__
	while(this->m_state != TIMER_STATE_IDLE)
	{
		if(this->m_state == TIMER_STATE_RUNNING)
		{
			this->m_exitFlag = true;
		}
		else if(this->m_state == TIMER_STATE_STOPPED)
		{
			this->m_exitFlag = true;
			if(this->m_thread != NULL)
			{
				::ResumeThread(this->m_thread);
			}
		}
		::Sleep(10);
	}
	::CloseHandle(this->m_thread);
	this->m_thread = NULL;
#elif __linux__

#endif
	this->m_mutex.unlock();
}

#ifdef _WINDOWS
DWORD WINAPI Timer::TimerHandler(LPVOID lpParamter)
{
	Timer* pThread = (Timer*)lpParamter;
	if (pThread != NULL)
	{
		pThread->m_state = TIMER_STATE_RUNNING;
		pThread->execute();
	}
	return 0;
}
#endif

void Timer::onTimer(Timer* pTimer)
{
	
}
