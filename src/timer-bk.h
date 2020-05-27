#ifndef _ICK_TIMER_H_
#define _ICK_TIMER_H_

#include "src/object.h"
#ifdef __windows__
#include <Windows.h>
#elif __linux__

#endif
#include "src/mutex.h"


class ick_api Timer : public IObject
{
public:
  typedef enum
  {
    TIMER_STATE_IDLE = 0,
    TIMER_STATE_RUNNING,
    TIMER_STATE_STOPPED,
  }TimerState;

  typedef enum
  {
    TIMER_PRIORITY_INHERIT,
    TIMER_PRIORITY_LOWEST,
    TIMER_PRIORITY_LOW,
    TIMER_PRIORITY_NORMAL,
    TIMER_PRIORITY_HIGH,
    TIMER_PRIORITY_HIGHEST
  }TimerPriority;

public:

  Timer();

  virtual ~Timer();

protected:

  static int timerId;

  int m_timerId;

  bool m_exitFlag;

  HANDLE m_thread;

  int m_interval;

  Timer* m_parent;

  TimerState m_state;

  IMutex m_mutex;
#ifdef __windows__
  static DWORD WINAPI TimerHandler(LPVOID lpParamter);
#elif __linux__

#endif
  virtual void execute();

public:

  void setInterval(int millisecond);

  void setParent(Timer* parent);

  void setTimerId(int timerId);

  int getTimerId();

  virtual void onTimer(Timer* pTimer);

public:

  void start();

  void stop();

  void exit();

};

#endif
