#include "src/converter.h"
#ifdef __windows__
#include "Windows.h"
#elif __linux__
#include <string.h>
#include <stdlib.h>
#endif

#include <sstream>

#include "src/logger.h"

Converter::Converter() {}


Converter::~Converter() {}

std::string Converter::convertToString(int value)
{
  char buf[32] = {0};
  sprintf(buf,"%d", value);
  return std::string(buf);
}

static std::string convertToString(int value, std::string& format)
{
  char buf[32] = {0};
  sprintf(buf, format.c_str(), value);
  return std::string(buf);
}

std::string Converter::convertToString(long value)
{
  char buf[256];
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%ld", value);
  return std::string(buf);
}

std::string Converter::convertToString(long long value)
{
  char buf[256];
  memset(buf, 0, sizeof(buf));
  sprintf(buf, "%lld", value);
  return std::string(buf);
}
std::string Converter::convertToString(double value, int accuracy)
{
  char buf[256];
  memset(buf, 0, sizeof(buf));
  if(accuracy < 0)
  {
    sprintf(buf, "%lf", value);
  }
  else if(accuracy >= 0)
  {
    std::string str = std::string("%.") + convertToString(accuracy) + std::string("f");
    sprintf(buf, str.c_str(), value);
  }
  return std::string(buf);
}

std::string Converter::convertToString(unsigned char value, bool hex)
{
  char buff[3];
  memset(buff, 0, sizeof(buff));
  char s1 = value / 16;
  char s2 = value % 16;
  buff[0] = (s1 < 10) ? s1 + '0' : s1 - 10 + 'A';
  buff[1] = (s2 < 10) ? s2 + '0' : s2 - 10 + 'A';
  return buff;
}

std::string Converter::convertToString(const unsigned char* buff, int length, bool hex)
{
  std::string res = "";
  if (buff == NULL || length == 0) return "";
  for (int i = 0; i < length; i++)
  {
    char d1 = buff[i] / 16;
    char d2 = buff[i] % 16;

    char s1[2] = { (d1 < 10) ? d1 + '0' : (d1 - 10) + 'A' , 0 };
    char s2[2] = { (d2 < 10) ? d2 + '0' : (d2 - 10) + 'A' , 0 };
    res += s1;
    res += s2;

    if (i < length - 1) res += " ";
  }

  return res;
}

std::string Converter::convertToUtf8(std::string& str)
{
#ifdef __windows__
  int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

  wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
  ZeroMemory(pwBuf, nwLen * 2 + 2);

  ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

  int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

  char * pBuf = new char[nLen + 1];
  ZeroMemory(pBuf, nLen + 1);

  ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

  std::string retStr(pBuf);

  delete[]pwBuf;
  delete[]pBuf;

  pwBuf = NULL;
  pBuf = NULL;
#elif __linux__
  std::string retStr="";
#endif
  return retStr;
}

std::string Converter::convertToUtf8(const char* gb2312, std::string code)
{
  if(code.compare("gb2312") == 0)
  {
#ifdef __windows__
    int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len+1];
    memset(wstr, 0, len+1);
    MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len+1];
    memset(str, 0, len+1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    if(wstr) delete[] wstr;
    std::string res = std::string(str);
    if(str) delete[] str;
    return res;
#elif __linux__
    return "";
#endif
  }
  else
  {
    return std::string("");
  }
}

std::string Converter::utf8ToString(std::string& str)
{
#ifdef __windows__
  int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

  wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
  memset(pwBuf, 0, nwLen * 2 + 2);

  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

  int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

  char * pBuf = new char[nLen + 1];
  memset(pBuf, 0, nLen + 1);

  WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

  std::string retStr = pBuf;

  delete[]pBuf;
  delete[]pwBuf;

  pBuf = NULL;
  pwBuf = NULL;

  return retStr;
#elif __linux__
  return "";
#endif
}

std::vector<std::string> Converter::split(std::string str, std::string pattern)
{
  std::string::size_type pos;
  std::vector<std::string> result;

  std::string s = trim(str);
  while ((pos = s.find(pattern)) != std::string::npos)
  {
    std::string tmp = s.substr(0, pos);
    // if (tmp != "") result.push_back(tmp);
    result.push_back(tmp);
    s = trim(s.substr(pos + 1, s.size() - pos));
    if(s.compare(pattern) == 0)
    {
      result.push_back("");
      break;
    }
  }

  if (s != "")
  {
    result.push_back(s);
  }

  return result;
}

std::string Converter::trim(std::string s)
{
  if (s == "") return "";
  std::string str = s;
  while (str[0] == ' ')
  {
    str = str.substr(1, str.size() - 1);
  }

  while (str[str.size() - 1] == ' ')
  {
    str = str.substr(0, str.size() - 1);
  }

  return str;
}

std::string Converter::replace(
    const std::string& str,
    const std::string& str_src,
    const std::string& str_dst) {
  std::string::size_type pos = 0;
  std::string::size_type len_src = str_src.size();
  std::string::size_type len_dst = str_dst.size();

  std::string result = str;
  pos = result.find(str_src, pos);
  while (pos != std::string::npos) {
    result.replace(pos, len_src, str_dst);
    pos = result.find(str_src, pos + len_dst);
  }
  return result;
}

std::string Converter::encodeUrl(const std::string& str) {
  std::string res = "";
  for (int i = 0; i < str.length(); ++i) {
    char data = str[i];
    if (data == ' ') {
      res += '+';
      continue;
    }
    if (data >= 0) {
      res += data;
      continue;
    }
    unsigned char tmp = (unsigned char)data;
    int data1 = tmp / 16;
    int data2 = tmp % 16;
    data1 = data1 > 9 ? (data1 - 10 + 'A') : (data1 + '0');
    data2 = data2 > 9 ? (data2 - 10 + 'A') : (data2 + '0');
    char buf[4] = { '%', (char)data1, (char)data2, '\0' };
    res += std::string(buf);
  }
  return res;
}

std::string Converter::decodeUrl(const std::string& str) {

}

std::string Converter::convertUtf16ToUtf8(const uint16_t* data) {
  if (data == nullptr) return "";
  std::stringstream result;
  int i = 0;
  while (true) {
    uint16_t d = data[i];
    if (d == 0) break;
    if (d <= 0x0000007F) {
      std::string s(1, 0);
      s[0] = (char)(d & 0x7F);
      result << s;
    } else if (d <= 0x000007FF) {
      std::string s(2, 0);
      s[0] = (char)(0xC0 | ((d >> 6) & 0x1F));
      s[1] = (char)(0x80 | (d & 0x3F));
      result << s;
    } else if (d <= 0x000FFFF) {
      std::string s(3, 0);
      s[0] = (char)(0xE0 | ((d >> 12) & 0x0F));
      s[1] = (char)(0x80 | ((d >> 6) & 0x3F));
      s[2] = (char)(0x80 | (d & 0x3F));
      result << s;
    } else if (d <= 0x001FFFFF) {
      std::string s(4, 0);
      s[0] = (char)(0xF0 | ((d >> 18) & 0x07));
      s[1] = (char)(0x80 | ((d >> 12) & 0x3F));
      s[2] = (char)(0x80 | ((d >> 6) & 0x3F));
      s[3] = (char)(0x80 | (d & 0x3F));
      result << s;
    } else if (d <= 0x03FFFFFF) {
      std::string s(5, 0);
      s[0] = (char)(0xF8 | ((d >> 24) & 0x03));
      s[1] = (char)(0x80 | ((d >> 18) & 0x3F));
      s[2] = (char)(0x80 | ((d >> 12) & 0x3F));
      s[3] = (char)(0x80 | ((d >> 6) & 0x3F));
      s[4] = (char)(0x80 | (d & 0x3F));
      result << s;
    } else {
      std::string s(6, 0);
      s[0] = (char)(0xFC | ((d >> 30) & 0x01));
      s[1] = (char)(0x80 | ((d >> 24) & 0x3F));
      s[2] = (char)(0x80 | ((d >> 18) & 0x3F));
      s[3] = (char)(0x80 | ((d >> 12) & 0x3F));
      s[4] = (char)(0x80 | ((d >> 6) & 0x3F));
      s[5] = (char)(0x80 | (d & 0x3F));
      result << s;
    }
    i++;
  }
  return result.str();
}

void Converter::convertUtf8ToUtf16(const std::string& str, std::vector<uint16_t>* res) {
  if (str.empty() || res == nullptr) return;
  const char* data = str.c_str();
  for (int i = 0; i < str.length();) {
    unsigned char ch0 = data[i + 0];
    unsigned char ch1 = data[i + 1];
    unsigned char ch2 = data[i + 2];
    unsigned char ch3 = data[i + 3];
    unsigned char ch4 = data[i + 4];
    unsigned char ch5 = data[i + 5];
    uint16_t s = 0;
    if (ch0 >> 7 == 0) {
      s = ch0 & 0x7F;
      i += 1;
    } else if (ch0 >> 5 == 6 && ch1 >> 6 == 2) {
      s = (((uint16_t)ch0 & 0x1F) << 6) + (ch1 & 0x3F);
      i += 2;
    } else if (ch0 >> 4 == 14 && ch1 >> 6 == 2 && ch2 >> 6 == 2) {
      s = (((uint16_t)ch0 & 0x0F) << 12) + ((ch1 & 0x3F) << 6) + (ch2 & 0x3F);
      i += 3;
    } else if (ch0 >> 3 == 0x1E && ch1 >> 6 == 2 && ch2 >> 6 == 2 && ch3 >> 6 == 2) {
      s = (((uint16_t)ch0 & 0x07) << 18) + ((ch1 & 0x3F) << 12) + ((ch2 & 0x3F) << 6) + (ch3 & 0x3F);
      i += 4;
    } else if (ch0 >> 2 == 0x3E && ch1 >> 6 == 2 && ch2 >> 6 == 2 && ch3 >> 6 == 2 && ch4 >> 6 == 2) {
      s = (((uint16_t)ch0 & 0x03) << 24) + ((ch1 & 0x3F) << 18) + ((ch2 & 0x3F) << 12) + ((ch3 & 0x3F) << 6) + (ch4 & 0x3F);
      i += 5;
    } else if (ch0 >> 2 == 0x7E && ch1 >> 6 == 2 && ch2 >> 6 == 2 && ch3 >> 6 == 2 && ch4 >> 6 == 2 && ch5 >> 6 == 2) {
      s = (((uint16_t)ch0 & 0x01) << 30) + ((ch1 & 0x3F) << 24) + ((ch2 & 0x3F) << 18) + ((ch3 & 0x3F) << 12) + ((ch4 & 0x3F) << 6) + (ch5 & 0x3F);
      i += 6;
    } else {
      Logger::getInstance()->Error("utf8 format error!!");
      return;
    }
    res->push_back(s);
  }
}
