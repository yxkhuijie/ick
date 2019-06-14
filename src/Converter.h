#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include "IObject.h"
#include <vector>
class Converter : public IObject
{
private:
  Converter();
  ~Converter();

public:

  static std::string convertToString(int value);
  static std::string convertToString(int value, std::string& format);
  static std::string convertToString(long value);
  static std::string convertToString(long long value);
  static std::string convertToString(double value, int accuracy = -1);
  static std::string convertToString(unsigned char value, bool hex = true);
  static std::string convertToString(const unsigned char* buff, int length, bool hex = true);

  static std::vector<std::string> split(std::string str, std::string pattern);
  static std::string trim(std::string s);
  static std::string convertToUtf8(std::string& str);
  static std::string convertToUtf8(const char* gb2312, std::string code=std::string("gb2312"));
  static std::string utf8ToString(std::string& str);
};

#endif

