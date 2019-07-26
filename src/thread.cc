#include "src/thread.h"
#include <functional>
#include "src/logger.h"

IThread::IThread()
    : exit_flag_(false) {
#ifdef __windows__
  this->m_thread = NULL;
#elif defined __linux__
  this->thread_ = 0;
#endif
}

IThread::~IThread() {}

void IThread::start() {
  this->exit_flag_ = false;
#ifdef __windows__
  m_thread = CreateThread(NULL, 0, ThreadHandler, this, 0, NULL);
#elif __linux__
  int error = pthread_create(&thread_, NULL, &IThread::ThreadHandler, (void*)this);
  if (error != 0) {
    this->exit_flag_ = true;
    Logger::getInstance()->Fatal("pthread_create failure!!!");
  }
#endif
}

#ifdef __windows__
DWORD WINAPI IThread::ThreadHandler(LPVOID lpParamter) {
  IThread* pThread = (IThread*)lpParamter;
  if (pThread != NULL) {
    pThread->m_state = THREAD_RUNNING;
    pThread->execute();
  }
  return 0;
}
#elif __linux__
void* IThread::ThreadHandler(void* arg) {
  IThread* thread = (IThread*)arg;
  if (thread != NULL) {
    thread->m_state = THREAD_RUNNING;
    thread->execute();
  }
}
#endif

void IThread::stop() {
  this->exit_flag_ = true;
  this->join();
}

void IThread::resume() {
#ifdef __windows__  // resume thread for windows
  if (this->m_thread != NULL) {
    ::ResumeThread(this->m_thread);
  }
#elif __linux__
  if(this->thread_ != NULL) {
  
  }
#endif
}

void IThread::join() {
#ifdef __windows__
#elif __linux__
  void *tret;
  int err = pthread_join(this->thread_, &tret);
  if(err != 0) {
    Logger::getInstance()->Fatal("pthread_join failure!!!");
  }
#endif
}
