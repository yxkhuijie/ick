#ifndef _ITHREAD_H_
#define _ITHREAD_H_

#include "IObject.h"

#ifdef _WINDOWS
#include <Windows.h>
#endif

typedef enum
{
  THREAD_NONE = 0,
  THREAD_RUNNING,
  THREAD_FINISHED
}ThreadState;

typedef enum
{
  PRIORITY_INHERIT,
  PRIORITY_LOWEST,
  PRIORITY_LOW,
  PRIORITY_NORMAL,
  PRIORITY_HIGH,
  PRIORITY_HIGHEST
}ThreadPriority;

class ick_api IThread : public IObject
{

private:
  HANDLE m_thread;

private:
  ThreadState m_state;

public:
  IThread();
  virtual ~IThread();

  void start();
  void stop();
  void resume();

    #ifdef __windows__
  static DWORD WINAPI ThreadHandler(LPVOID lpParamter);
    #else
  
    #endif

  virtual void execute() = 0;
};

#endif
