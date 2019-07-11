#include "src/logger.h"
#include <fstream>
#ifdef __windows__
#include <io.h>
#elif __linux__
#include <unistd.h>
#endif
#include "src/converter.h"
#include "src/file.h"
#include "absl/time/time.h"
#include "absl/time/clock.h"

Logger* Logger::m_instance = new Logger();

Logger::Logger() {
  m_fileSize = 0;
  m_filePath = "";
  m_logLevel = ALL;
  m_logFileName = "log";
  wchar_t szFilePath[MAX_PATH] = { 0 };
  char filePath[MAX_PATH] = { 0 };
#ifdef __windows__
#ifdef _UNICODE
  GetModuleFileName(NULL, szFilePath, sizeof(filePath));
  (wcsrchr(szFilePath, L'\\'))[1] = 0;

  int len = WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, filePath, 0, NULL, NULL);
  WideCharToMultiByte(CP_ACP, 0, szFilePath, -1, filePath, len, NULL, NULL);

  this->m_filePath = std::string(filePath) + "log\\";

  len = MultiByteToWideChar(CP_ACP, 0, m_filePath.c_str(), -1, szFilePath, 0);
  MultiByteToWideChar(CP_ACP, 0, m_filePath.c_str(), -1, szFilePath, len);
#else
  m_filePath = std::string(filePath).substr(0, std::string(filePath).find_last_of("\\")) + "\\log\\";
  int len = MultiByteToWideChar(CP_ACP, 0, m_filePath.c_str(), -1, szFilePath, 0);
  MultiByteToWideChar(CP_ACP, 0, m_filePath.c_str(), -1, szFilePath, len);
#endif

  if (::_access(m_filePath.c_str(), 0) < 0)
  {
    if (::_wmkdir(szFilePath) < 0)
    {
      printf("create log folder failure!");
    }
  }
#elif __linux__
  m_filePath="/tmp/";
#endif
  this->file_index_ = 0;
  this->setFilePath(m_filePath);
}

Logger::~Logger() {}

Logger* Logger::getInstance() {
  return m_instance;
}

void Logger::startup() {
  this->Debug(std::string("Logger has been startup!"));
  this->start();
}

void Logger::setFilePath(std::string path) {
#ifdef __windows__
  if (path.at(path.size() - 1) != '\\') {
    path += std::string("\\");
  }
#elif __linux__
  if (path.at(path.size() - 1) != '/') {
    path += std::string("/");
  }
#endif

#ifdef __windows__
  if (::_access(path.c_str(), 0) == -1)
#elif __linux__
  if (::access(path.c_str(), 0) != 0)
#endif  
  {
    printf("Logerr path not exist!");
    // 采用默认路径
    return;
  }

  this->m_filePath = path;
}

void Logger::Log(const std::string& message, const std::string& log_pos, LoggerLevel level, LoggerType type) {
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, "",level, type);
  if (type != Record) {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && level >= this->m_logLevel) {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}

void Logger::Debug(const std::string& message, const std::string& log_pos, LoggerType type) {
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, "",DEBUG, type);
  if (type != Record) {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && DEBUG >= this->m_logLevel) {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}

void Logger::Info(const std::string& message, const std::string& log_pos, LoggerType type) {
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, "",INFO, type);
  if (type != Record) {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && INFO >= this->m_logLevel) {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}

void Logger::Warn(const std::string& message, const std::string& log_pos, LoggerType type) {
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, "",WARN, type);
  if (type != Record) {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && WARN >= this->m_logLevel) {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}

void Logger::Error(const std::string& message, const std::string& log_pos, LoggerType type) {
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, "",ERROR, type);
  if (type != Record) {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && ERROR >= this->m_logLevel) {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}

void Logger::Fatal(const std::string& message, const std::string& log_pos, LoggerType type) {
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, "", FATAL, type);
  if (type != Record) {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && FATAL >= this->m_logLevel) {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}

void Logger::execute() {
  std::ofstream myfile;
  while (true) {
    m_mutex.lock();
    std::list<LoggerMessage> logger_message_list;
    logger_message_list.assign(m_messages.begin(), m_messages.end());
    m_messages.clear();
    m_mutex.unlock();
    if (logger_message_list.empty()) {
#ifdef __windows__    
      Sleep(100);
#elif __linux__
      usleep(100000);
#endif
      continue;
    }
    if (file_name_.empty()) {
      std::string file_name = m_filePath + "log%Y%m%dT%H%M%E9S";
      if (file_index_ > 0) {
        file_name += "_" + std::to_string(file_index_);
      }
      file_name += ".txt";
      file_name_ = absl::FormatTime(file_name, absl::Now(), absl::LocalTimeZone());
      HistoryLoggerInfo history_logger_info;
      history_logger_info.file_name_ = file_name_;
      history_logger_info.line_count_ = 0;
      history_logger_info.file_size_ = 0;
      logger_history_list_.push_back(history_logger_info);
    }
    mutex_logger_request_.lock();
    myfile.open(file_name_.c_str(), std::ios::app);
    for (auto it = logger_message_list.begin(); it != logger_message_list.end(); ++it) {
      myfile.write(it->toString().c_str(), it->toString().size());
    }

    size_t file_size = myfile.tellp();
    if (file_size >= 1048576) {
      this->file_index_++;
      this->file_name_ = "";
    }
    auto it1 = logger_history_list_.end();
    --it1;
    it1->line_count_ += logger_message_list.size();
    it1->file_size_ = file_size;
    for (const auto& it2 : logger_subscriber_list_) {
      it2->update(logger_message_list);
    }
    logger_message_list.clear();
    myfile.close();
    mutex_logger_request_.unlock();
#ifdef __windows__    
    Sleep(100);
#elif __linux__
    usleep(100000);
#endif
  }
}

void Logger::getHistoryLog(std::list<std::string>* data, int count /* = 0 */, LoggerSubscriber* subscriber ) {
  if (data == nullptr) return;
  int total_line_count = 0;
  for (const auto& it : logger_history_list_) {
    total_line_count += it.line_count_;
  }
  if (total_line_count <= 0) {
    Logger::getInstance()->Error("get history log error: total line count: " + std::to_string(total_line_count));
    return;
  }
  int start_line_index = 0;
  if (count > 0 && count < total_line_count) {
    start_line_index = total_line_count - count;
  }
  int total_line_index = 0;
  mutex_logger_request_.lock();
  for (const auto& it : logger_history_list_) {
    if (it.file_name_.empty()) continue;
    if (total_line_index + it.line_count_ < start_line_index) {
      total_line_index += it.line_count_;
      continue;
    }
    int index = start_line_index - total_line_index;
    std::list<std::string> lst;
    bool success = File::readFile(it.file_name_, &lst, index);
    if (success && !lst.empty()) {
      total_line_index += lst.size();
      if (data->empty()) {
        data->assign(lst.begin(), lst.end());
      } else {
        data->insert(data->end(), lst.begin(), lst.end());
      }
    } else if (!success) {
      this->Warn("get history log error: " + it.file_name_);
    } else {
      this->Error("get history log error: unknown error");
    }
  }
  if (subscriber != nullptr) {
    logger_subscriber_list_.push_back(subscriber);
  }
  mutex_logger_request_.unlock();
}

void Logger::setLoggerSubscriber(LoggerSubscriber* subscriber) {
  if (subscriber == nullptr) return;
  mutex_logger_request_.lock();
  logger_subscriber_list_.push_back(subscriber);
  mutex_logger_request_.unlock();
}

void Logger::cancelLoggerSubscriber(LoggerSubscriber* subscriber) {
  if (subscriber == nullptr) return;
  mutex_logger_request_.lock();
  std::list<LoggerSubscriber*>::iterator it;
  for (it = logger_subscriber_list_.begin(); it != logger_subscriber_list_.end(); ++it) {
    if (*it == subscriber) {
      logger_subscriber_list_.erase(it);
      break;
    }
  }
  mutex_logger_request_.unlock();
}
