#include "src/time_converter.h"
#ifdef __linux__
#include <stdlib.h>
#endif

TimeConverter::TimeConverter()
{

}

std::string TimeConverter::getCurrentTimeAsStr()
{
#ifdef __windows__
  SYSTEMTIME sys;
  GetLocalTime(&sys);
  char tmp[64];
  sprintf(tmp, "%4d/%02d/%02d %02d:%02d:%02d.%03d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
  return std::string(tmp);
#elif __linux__
  return std::string("");
#endif
}

std::string TimeConverter::getCurrentTimeAsStr(const std::string& format, int days)
{
#ifdef __windows__
  SYSTEMTIME sys;
  GetLocalTime(&sys);
  char tmp[64] = { 0 };
  if (format.compare("yyyy-mm-dd hh:mm:ss") == 0)
  {
    sprintf(tmp, "%4d-%02d-%02d %02d:%02d:%02d", sys.wYear, sys.wMonth, sys.wDay - days, sys.wHour, sys.wMinute, sys.wSecond);
    return std::string(tmp);
  }
  else if (format.compare("yyyy-mm-dd 00:00:00") == 0)
  {
    sprintf(tmp, "%4d-%02d-%02d 00:00:00", sys.wYear, sys.wMonth, sys.wDay - days);
    return std::string(tmp);
  }
  else if (format.compare("yyyy-MM-dd") == 0 || format.compare("yyyy-mm-dd") == 0)
  {
    sprintf(tmp, "%4d-%02d-%02d", sys.wYear, sys.wMonth, sys.wDay - days);
    return std::string(tmp);
  }
  else if(format.compare("yyyyMMdd") == 0 || format.compare("yyyymmdd") == 0)
  {
    sprintf(tmp, "%4d%02d%02d", sys.wYear, sys.wMonth, sys.wDay - days);
    return std::string(tmp);
  }
  else
  {
    sprintf(tmp, "%4d-%02d-%02d %02d:%02d:%02d", sys.wYear, sys.wMonth, sys.wDay - days, sys.wHour, sys.wMinute, sys.wSecond);
    return std::string(tmp);
  }
#elif __linux
  return std::string("");
#endif
}

long TimeConverter::getCurrentTimeAsSeconds()
{
#ifdef __windows__
  time_t tt = time(NULL);//这句返回的只是一个时间cuo
  
  /*
  tm* t = localtime(&tt);
  
  printf("%d-%02d-%02d %02d:%02d:%02d\n",
    t->tm_year + 1900,
    t->tm_mon + 1,
    t->tm_mday,
    t->tm_hour,
    t->tm_min,
    t->tm_sec);
  */
  return tt;
#elif __linux
  return 0;
#endif
}

long TimeConverter::getTimeFromStringAsSeconds(const std::string& str, const std::string& format)
{
  char szYear[5], szMonth[3], szDay[3], szHour[3], szMin[3], szSec[3];

  const char* pDate = str.c_str();

  szYear[0] = *pDate++;
  szYear[1] = *pDate++;
  szYear[2] = *pDate++;
  szYear[3] = *pDate++;
  szYear[4] = 0x0;

  ++pDate;

  szMonth[0] = *pDate++;
  szMonth[1] = *pDate++;
  szMonth[2] = 0x0;

  ++pDate;

  szDay[0] = *pDate++;
  szDay[1] = *pDate++;
  szDay[2] = 0x0;

  ++pDate;

  szHour[0] = *pDate++;
  szHour[1] = *pDate++;
  szHour[2] = 0x0;

  ++pDate;

  szMin[0] = *pDate++;
  szMin[1] = *pDate++;
  szMin[2] = 0x0;

  ++pDate;

  szSec[0] = *pDate++;
  szSec[1] = *pDate++;
  szSec[2] = 0x0;

  tm tmObj;

  tmObj.tm_year = atoi(szYear) - 1900;
  tmObj.tm_mon = atoi(szMonth) - 1;
  tmObj.tm_mday = atoi(szDay);
  tmObj.tm_hour = atoi(szHour);
  tmObj.tm_min = atoi(szMin);
  tmObj.tm_sec = atoi(szSec);
  tmObj.tm_isdst = -1;
#ifdef __windows__
  return mktime(&tmObj);
#elif __linux__
  return 0;
#endif
}

int TimeConverter::getDaysOfMonth(int year, int month)
{
  if(month <= 0 || month > 12) return 0;

  int day[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
  int d = 0;
  if (2 == month)
  {
    d = (((0 == year % 4) && (0 != year % 100) || (0 == year % 400)) ? 29 : 28);
  }
  else
  {
    d = day[month - 1];
  }

  return d;
}

long long TimeConverter::getCurrentTimeAsMilliseconds()
{
#ifdef __windows__
  struct timeval tv;
  time_t clock;
  struct tm tm;
  SYSTEMTIME wtm;

  GetLocalTime(&wtm);
  tm.tm_year = wtm.wYear - 1900;
  tm.tm_mon = wtm.wMonth - 1;
  tm.tm_mday = wtm.wDay;
  tm.tm_hour = wtm.wHour;
  tm.tm_min = wtm.wMinute;
  tm.tm_sec = wtm.wSecond;
  tm.tm_isdst = -1;
  clock = mktime(&tm);
  tv.tv_sec = clock;
  tv.tv_usec = wtm.wMilliseconds * 1000;
  return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
#elif __linux__
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
#endif  
}

long long TimeConverter::getCurrentTimeAsMicroseconds()
{
#ifdef __windows__
  // 从1601年1月1日0:0:0:000到1970年1月1日0:0:0:000的时间(单位100ns)
#define EPOCHFILETIME   (116444736000000000UL)
  FILETIME ft;
  LARGE_INTEGER li;
  long long tt = 0;
  GetSystemTimeAsFileTime(&ft);
  li.LowPart = ft.dwLowDateTime;
  li.HighPart = ft.dwHighDateTime;
  // 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
  tt = (li.QuadPart - EPOCHFILETIME) / 10;
  return tt;
#elif __linux__
  timeval tv;
  gettimeofday(&tv, 0);
  return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif // __WINDOWS
}

std::string TimeConverter::getCurrentTime(const TimeItem &t)
{
#ifdef __windows__
  SYSTEMTIME sys;
  GetLocalTime(&sys);
  t.m_day = sys.wDay;
  t.m_dayOfWeek = sys.wDayOfWeek;
  t.m_hour = sys.wHour;
  t.m_minute = sys.wMinute;
  t.m_month = sys.wMonth;
  t.m_year = sys.wYear;
  t.m_sec = sys.wSecond;
  t.m_msec = sys.wMilliseconds;

  char tmp[64] = { 0 };
  sprintf(tmp, "%04d-%02d-%02d %02d:%02d:%02d", 
    t.m_year, t.m_month, t.m_day, 
    t.m_hour, t.m_minute, t.m_sec);
  return std::string(tmp);
#elif __linux__
  return std::string("");
#endif
}

std::string TimeConverter::timeToString(const TimeItem &t, bool format)
{
  return "";
}

std::string TimeConverter::getTimeFromTimeStamp(long timeStamp)
{
#ifdef __windows__
  time_t tick = (time_t)timeStamp;
  struct tm * timeinfo;
  timeinfo = localtime(&tick);
  char tmp[64];
  sprintf(tmp, "%4d/%02d/%02d %02d:%02d:%02d", 
    timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, 
    timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  return std::string(tmp);
#elif __linux__
  return std::string("");
#endif
}

std::string TimeConverter::getTimeFromTimeStamp(long timeStamp, const TimeItem& t)
{
#ifdef __windows__
  time_t tick = (time_t)timeStamp;
  struct tm * timeinfo;
  timeinfo = localtime(&tick);

  t.m_year = timeinfo->tm_year + 1900;
  t.m_month = timeinfo->tm_mon + 1;
  t.m_day = timeinfo->tm_mday;
  t.m_hour = timeinfo->tm_hour;
  t.m_minute = timeinfo->tm_min;
  t.m_sec = timeinfo->tm_sec;
  t.m_dayOfWeek = timeinfo->tm_wday;

  char tmp[64];
  sprintf(tmp, "%4d/%02d/%02d %02d:%02d:%02d",
    timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
    timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  return std::string(tmp);
#elif __linux__
  return std::string("");
#endif
}

std::string TimeConverter::getTimeFromTimeItem(const TimeConverter::TimeItem &t, const std::string& format)
{
  char tmp[64] = { 0 };
  sprintf(tmp, "%4d/%02d/%02d %02d:%02d:%02d", 
    t.m_year, t.m_month, t.m_day, 
    t.m_hour, t.m_minute, t.m_sec);
  return std::string(tmp);
}

long TimeConverter::getTimeStampFromTimeItem(const TimeConverter::TimeItem &t)
{
  std::string strTime = getTimeFromTimeItem(t);
  return getTimeFromStringAsSeconds(strTime);
}

TimeConverter::TimeItem TimeConverter::getTimeFromString(const std::string& str, const std::string& format)
{
  TimeConverter::TimeItem timeItem;
  if (format.compare(std::string("yyyy-MM-dd HH:mm:ss")) == 0)
  {
    sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &timeItem.m_year, 
      &timeItem.m_month, &timeItem.m_day, &timeItem.m_hour, &timeItem.m_minute, &timeItem.m_sec);
  }
  return timeItem;
}
