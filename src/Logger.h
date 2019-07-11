#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "src/thread.h"
#include "src/mutex.h"
#include <list>
#include "src/time_converter.h"

#if (defined(__linux__)&&!defined(MAX_PATH))
#define MAX_PATH 260
#endif

enum LoggerType {
  RecordAndPrint = 0,
  Record = 1,
  Print = 2
};

enum LoggerLevel {
  ALL = 0,
  DEBUG = 1,
  INFO = 2,
  WARN = 3,
  ERROR = 4,
  FATAL = 5,
  OFF = 6
};

struct HistoryLoggerInfo {
 public:
  std::string file_name_;
  int line_count_;
  int file_size_;
};

class LoggerMessage {
 public:
  std::string logTime;
  std::string logMessage;
  LoggerLevel logLevel;
  LoggerType logType;
  std::string strLevel;
  std::string logPos;

 public:
  LoggerMessage() {}
  LoggerMessage(const std::string& msg, const std::string& pos = "",
      LoggerLevel level = ALL, LoggerType print = RecordAndPrint) {
    this->logTime = TimeConverter::getCurrentTimeAsStr();
    this->logMessage = msg;
    this->logLevel = level;
    this->logType = print;
    this->logPos = pos;
    if (level == ALL) strLevel = "ALL";
    else if (level == DEBUG) strLevel = "DEBUG";
    else if (level == INFO) strLevel = "INFO";
    else if (level == WARN) strLevel = "WARN";
    else if (level == ERROR) strLevel = "ERROR";
    else if (level == FATAL) strLevel = "FATAL";
    else if (level == OFF) strLevel = "OFF";
  };
  ~LoggerMessage() {};

  std::string toString(bool crlf = true) {
    if (logLevel == WARN || logLevel == ERROR || logLevel == FATAL) {
      return "!! " + logTime + " " + strLevel + "    " + logMessage + (crlf ? "\n" : "");
    } else {
      return logTime + "    " + strLevel + "    " + logMessage + (crlf ? "\n" : "");
    }
  }

  std::string toPrintString() {
    return logTime + ": " + logMessage + "\n";
  }

  void print() {
    if (logLevel == WARN || logLevel == ERROR || logLevel == FATAL) {
      std::cout << "!! " + logTime + ": " + logMessage << std::endl;
    } else {
      std::cout << logTime + ": " + logMessage << std::endl;
    }
  }
};

class ick_api LoggerSubscriber {
 public:
  virtual void update(const std::list<LoggerMessage>& logger_message_list) = 0;
};

class ick_api Logger : public IThread {
 public:
  Logger();
  ~Logger();

 private:
  static Logger* m_instance;
  IMutex m_mutex;
  IMutex mutex_logger_request_;
  std::string m_filePath;
  std::list<LoggerMessage> m_messages;
  long m_fileSize;
  int m_fileIndex;
  friend class IApplication;
  std::string m_logFileName;
  LoggerLevel m_logLevel;
  std::string file_name_;
  int file_index_;
  std::list<HistoryLoggerInfo> logger_history_list_;
  std::list<LoggerSubscriber*> logger_subscriber_list_;

 private:
  virtual void execute();

 public:
  void startup();
  static Logger* getInstance();
  void setFilePath(std::string path);
  void getHistoryLog(std::list<std::string>* data, int count = 0, LoggerSubscriber* subscriber = nullptr);
  void setLoggerSubscriber(LoggerSubscriber* subscriber);
  void cancelLoggerSubscriber(LoggerSubscriber* subscriber);

  void Log(const std::string& message,
      const std::string& log_pos = "",
      LoggerLevel level= ALL,
      LoggerType print = RecordAndPrint);
  void Debug(const std::string& message,
      const std::string& log_pos = "",
      LoggerType print = RecordAndPrint);
  void Info(const std::string& message,
      const std::string& log_pos = "",
      LoggerType print = RecordAndPrint);
  void Warn(const std::string& message,
      const std::string& log_pos = "",
      LoggerType print = RecordAndPrint);
  void Error(const std::string& message,
      const std::string& log_pos = "",
      LoggerType print = RecordAndPrint);
  void Fatal(const std::string& message,
      const std::string& log_pos = "",
      LoggerType print = RecordAndPrint);
};

#define LOG(type) \
  switch (type) {\
    case ALL: \
      Logger::getInstance()->Log(); \
      break; \
    case DEBUG: \
      Logger::getInstance()->Debug(); \
      break; \
    case INFO: \
      Logger::getInstance()->Info(); \
      break; \
    case WARN: \
      Logger::getInstance()->Warn(); \
      break; \
    case ERROR: \
      Logger::getInstance()->Error(); \
      break; \
    case FATAL: \
      Logger::getInstance()->Fatal(); \
      break; \
  }

#endif
