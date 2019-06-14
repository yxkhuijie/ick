#ifndef _TIME_CONVERTER_H_
#define _TIME_CONVERTER_H_

#include "IObject.h"

#ifdef __windows__ 
#   include <time.h>
#   include <Windows.h>
#elif __linux__
#   include <sys/time.h>
#endif

#ifdef __linux__
typedef long long int64_t;
#endif

class ick_api TimeConverter
{
public:
  class TimeItem
  {
  public:

    int m_year;
    int m_month;     // 1 ~ 12
    int m_dayOfWeek; // Sunday = 0, Monday = 1, and so on. 
    int m_day;       // day of month
    int m_hour;      // 0 ~ 23
    int m_minute;    // 0 ~ 59
    int m_sec;       // 0 ~ 59
    int m_msec;      // 0 ~ 999

    TimeItem() :m_year(0), m_month(0), m_dayOfWeek(0),
      m_day(0), m_hour(0), m_minute(0), m_sec(0), m_msec(0) {}

    ~TimeItem() {}
  };

protected:
  TimeConverter();

public:

  // Gets the current system time as string like "yyyy-mm-dd hh:mm:ss.ms"
  static std::string getCurrentTimeAsStr();

  // Gets the current system time as string for format
  // yyyy-mm-dd hh:mm:ss.ms
  static std::string getCurrentTimeAsStr(const std::string& format, int days = 0);

  // Gets the current system time as seconds since the Epoch (00:00:00 UTC, Jan 1, 1970)
  static long getCurrentTimeAsSeconds();

  static long long getCurrentTimeAsMilliseconds();

  static long long getCurrentTimeAsMicroseconds();

  // Gets current time as TimeItem
  static std::string getCurrentTime(const TimeItem &t);

  static std::string timeToString(const TimeItem &t, bool format);

  static std::string getTimeFromTimeStamp(long timeStamp);

  static std::string getTimeFromTimeStamp(long timeStamp, const TimeItem& t);

  static std::string getTimeFromTimeItem(const TimeItem &t, const std::string& format = std::string("yyyy-MM-dd HH:mm:ss"));

  static long getTimeStampFromTimeItem(const TimeItem &t);

  static TimeItem getTimeFromString(const std::string& str, const std::string& format = std::string("yyyy-MM-dd HH:mm:ss"));

  static long getTimeFromStringAsSeconds(const std::string& str, const std::string& format = std::string("yyyy-MM-dd HH:mm:ss"));

  static int getDaysOfMonth(int year, int month);
};

#endif

