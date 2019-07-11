#ifndef _ITHREAD_H_
#define _ITHREAD_H_

#include "src/object.h"

#ifdef __windows__
#include <Windows.h>
#elif __linux__
#include <pthread.h>
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
#ifdef __windows__
  HANDLE m_thread;
#elif __linux__
  pthread_t thread_;
#endif


private:
  ThreadState m_state;

public:
  IThread();
  virtual ~IThread();

  void start();
  void stop();
  void resume();
  void join();

#ifdef __windows__
  static DWORD WINAPI ThreadHandler(LPVOID lpParamter);
#elif __linux__
  static void* ThreadHandler(void* arg);
#endif

  virtual void execute() = 0;
};

#endif
