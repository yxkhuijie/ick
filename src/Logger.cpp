#include "Logger.h"
#include <fstream>
#ifdef __windows__
#include <io.h>
#endif
#include "Converter.h"


Logger* Logger::m_instance = new Logger();

Logger::Logger()
{
  m_fileSize = 0;
  m_filePath = "";
  m_logLevel = LOG_LEVEL_ALL;
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
  this->setFilePath(m_filePath);
}

Logger::~Logger()
{

}

Logger* Logger::getInstance()
{
  return m_instance;
}

void Logger::startup()
{
  this->Debug(std::string("Logger has been startup!"));
  this->start();
}

void Logger::setFilePath(std::string path)
{
  if (path.at(path.size() - 1) != '\\')
  {
    path += std::string("\\");
  }
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
  // 获取文件索引
  bool fileExist = true;
  this->m_fileIndex = 0;
  while (fileExist)
  {
    std::string fileName = m_filePath + "log" + TimeConverter::getCurrentTimeAsStr(std::string("yyyyMMdd")) + 
      (m_fileIndex == 0 ? "" : "-" + Converter::convertToString(m_fileIndex)) + ".txt";
#ifdef __windows__  
    if (::_access(path.c_str(), 0) >= 0)
#elif __linux__
    if (::access(path.c_str(), 0) >= 0)
#endif    
    {
      std::ifstream fsRead;
      fsRead.open(fileName.c_str(), std::ios::in | std::ios::binary);
      if (!fsRead) {
        printf("Uncompress processing: can not open source file! [%s]", fileName.c_str());
        return;
      }
      fsRead.seekg(0, fsRead.end);
      size_t srcSize = fsRead.tellg();
      if (!srcSize) {
        printf("Source file: [%s] size is 0! Return directly!", fileName.c_str());
        fsRead.close();
        return;
      }
      if (srcSize >= 1048576)
      {
        this->m_fileIndex++;
        continue;
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }
}

void Logger::Log(const std::string& message, LoggerLevel level, LoggerType type)
{
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, level, type);
  if (type != Record)
  {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && level >= this->m_logLevel)
  {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}

void Logger::Debug(const std::string& message, LoggerType type)
{
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, LOG_LEVEL_DEBUG, type);
  if (type != Record)
  {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && LOG_LEVEL_DEBUG >= this->m_logLevel)
  {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}
void Logger::Info(const std::string& message, LoggerType type)
{
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, LOG_LEVEL_INFO, type);
  if (type != Record)
  {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && LOG_LEVEL_INFO >= this->m_logLevel)
  {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}
void Logger::Warn(const std::string& message, LoggerType type)
{
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, LOG_LEVEL_WARN, type);
  if (type != Record)
  {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && LOG_LEVEL_WARN >= this->m_logLevel)
  {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}
void Logger::Error(const std::string& message, LoggerType type)
{
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, LOG_LEVEL_ERROR, type);
  if (type != Record)
  {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && LOG_LEVEL_ERROR >= this->m_logLevel)
  {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}
void Logger::Fatal(const std::string& message, LoggerType type)
{
  if (message.compare("") == 0) return;
  LoggerMessage loggerMessage(message, LOG_LEVEL_FATAL, type);
  if (type != Record)
  {
    m_mutex.lock();
    loggerMessage.print();
    m_mutex.unlock();
  }

  if (type != Print && LOG_LEVEL_FATAL >= this->m_logLevel)
  {
    m_mutex.lock();
    this->m_messages.push_back(loggerMessage);
    m_mutex.unlock();
  }
}

void Logger::execute()
{
  std::ofstream myfile;
  while (true)
  {
    m_mutex.lock();

    std::string fileName = this->m_filePath + "log" + TimeConverter::getCurrentTimeAsStr(std::string("yyyyMMdd"))+
      (m_fileIndex == 0 ? "" : ("-" + Converter::convertToString(this->m_fileIndex))) + ".txt";
    myfile.open(fileName.c_str(), std::ios::app);
    std::list<LoggerMessage>::iterator it;
    
    for (it = m_messages.begin(); it != m_messages.end(); ++it)
    {
      myfile.write(it->toString().c_str(), it->toString().size());
    }

    size_t dstFileSize = myfile.tellp();
    if (dstFileSize >= 1048576)
    {
      this->m_fileIndex++;
    }
    m_messages.clear();
    myfile.close();
    m_mutex.unlock();
#ifdef __windows__    
    Sleep(100);
#elif __linux__
    usleep(100000);
#endif
  }
}
