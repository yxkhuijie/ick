#include "src/thread.h"
#include <functional>

IThread::IThread()
{
#ifdef __windows__
  this->m_thread = NULL;
#elif defined __linux__
  this->m_thread = 0;
#endif
}

IThread::~IThread()
{

}

void IThread::start()
{
#ifdef _WINDOWS
  m_thread = CreateThread(NULL, 0, ThreadHandler, this, 0, NULL);
#endif
}

#ifdef _WINDOWS
DWORD WINAPI IThread::ThreadHandler(LPVOID lpParamter)
{
  IThread* pThread = (IThread*)lpParamter;
  if (pThread != NULL)
  {
    pThread->m_state = THREAD_RUNNING;
    pThread->execute();
  }
  return 0;
}
#endif

void IThread::stop()
{

}

void IThread::resume()
{
#ifdef __windows__  // resume thread for windows
  if (this->m_thread != NULL)
  {
    ::ResumeThread(this->m_thread);
  }
#elif __linux__
  if(this->m_thread != 0)
  {
  
  }
#endif
}
