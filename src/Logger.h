#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "src/thread.h"
#include "src/mutex.h"
#include <list>
#include "src/time_converter.h"

#if (defined(__linux__)&&!defined(MAX_PATH))
#define MAX_PATH 260
#endif


enum LoggerType
{
  RecordAndPrint = 0,
  Record = 1,
  Print = 2
};


enum LoggerLevel
{
  LOG_LEVEL_ALL = 0,
  LOG_LEVEL_DEBUG = 1,
  LOG_LEVEL_INFO = 2,
  LOG_LEVEL_WARN = 3,
  LOG_LEVEL_ERROR = 4,
  LOG_LEVEL_FATAL = 5,
  LOG_LEVEL_OFF = 6
};

class LoggerMessage
{
public:
  std::string logTime;
  std::string logMessage;
  LoggerLevel logLevel;
  LoggerType logType;
  std::string strLevel;

public:
  LoggerMessage(std::string msg, LoggerLevel level = LOG_LEVEL_ALL, LoggerType print = RecordAndPrint)
  {
    this->logTime    = TimeConverter::getCurrentTimeAsStr();
    this->logMessage = msg;
    this->logLevel   = level;
    this->logType    = print;
    if (level == LOG_LEVEL_ALL) strLevel = "ALL";
    else if (level == LOG_LEVEL_DEBUG) strLevel = "DEBUG";
    else if (level == LOG_LEVEL_INFO) strLevel = "INFO";
    else if (level == LOG_LEVEL_WARN) strLevel = "WARN";
    else if (level == LOG_LEVEL_ERROR) strLevel = "ERROR";
    else if (level == LOG_LEVEL_FATAL) strLevel = "FATAL";
    else if (level == LOG_LEVEL_OFF) strLevel = "OFF";
  };
  ~LoggerMessage() {};

  std::string toString() 
  {
    if (logLevel == LOG_LEVEL_WARN || logLevel == LOG_LEVEL_ERROR || logLevel == LOG_LEVEL_FATAL)
    {
      return "!! " + logTime + " " + strLevel + "    " + logMessage + "\n";
    }
    else
    {
      return logTime + "    " + strLevel + "    " + logMessage + "\n";
    }
  }

  std::string toPrintString()
  {
    return logTime + ": " + logMessage + "\n";
  }

  void print()
  {
    if (logLevel == LOG_LEVEL_WARN || logLevel == LOG_LEVEL_ERROR || logLevel == LOG_LEVEL_FATAL)
    {
      std::cout << "!! " + logTime + ": " + logMessage << std::endl;
    }
    else
    {
      std::cout << logTime + ": " + logMessage << std::endl;
    }
    
  }
};

class ick_api Logger : public IThread
{

public:

  Logger();

  ~Logger();

private:

  static Logger* m_instance;

  IMutex m_mutex;

  std::string m_filePath;

  virtual void execute();

  std::list<LoggerMessage> m_messages;

  long m_fileSize;

  int m_fileIndex;

  friend class IApplication;

  std::string m_logFileName;

  LoggerLevel m_logLevel;

public:

  void startup();

  void Log(const std::string& message, LoggerLevel level= LOG_LEVEL_ALL, LoggerType print = RecordAndPrint);

  void Debug(const std::string& message, LoggerType print = RecordAndPrint);
  void Info(const std::string& message, LoggerType print = RecordAndPrint);
  void Warn(const std::string& message, LoggerType print = RecordAndPrint);
  void Error(const std::string& message, LoggerType print = RecordAndPrint);
  void Fatal(const std::string& message, LoggerType print = RecordAndPrint);

  static Logger* getInstance();

  void setFilePath(std::string path);
};

#endif
