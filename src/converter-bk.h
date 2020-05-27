#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include "src/object.h"
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

  static std::vector<std::string> split(const std::string& str, const std::string& pattern);
  static int split(const std::string& str, const std::string& pattern, std::vector<std::string>* vec);
  static std::string trim(const std::string& s);
  static std::string replace(const std::string& str, const std::string& str_src, const std::string& str_dst);
  static std::string convertToUtf8(const std::string& str);
  static std::string convertToUtf8(const char* gb2312, const std::string& code="gb2312");
  static std::string utf8ToString(const std::string& str);
  static std::string encodeUrl(const std::string& str);
  static std::string decodeUrl(const std::string& str);

  static std::string convertUtf16ToUtf8(const uint16_t* data);
  static void convertUtf8ToUtf16(const std::string& str, std::vector<uint16_t>* res);
  static void SplitUtf8(const std::string& str, std::vector<std::string>* res);
};

#endif

