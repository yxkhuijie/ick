#include "src/converter.h"
#ifdef __windows__
#include "Windows.h"
#elif __linux__
#include <string.h>
#include <stdlib.h>
#endif



Converter::Converter()
{
}


Converter::~Converter()
{
}

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

std::string Converter::encodeUrl(const std::string& str) {
  std::string res = "";
  for (int i = 0; i < str.length(); ++i) {
    if (str[i] == ' ') {
      res += '+';
      continue;
    }
    unsigned int data = (unsigned int)str[i];
    if (data > 127) {
      char buf[4] = { '\0' };
      buf[0] = '%';
      buf[1] = data / 16;
      if (buf[1] > 9) buf[1] = buf[1] - 10 + 'A';
      else buf[1] = buf[1] + '0';
      buf[2] = data % 16;
      if (buf[2] > 9) buf[2] = buf[2] - 10 + 'A';
      else buf[2] = buf[2] + '0';
      res += buf;
    } else {
      res += str[i];
    }
  }
  return res;
}

std::string Converter::decodeUrl(const std::string& str) {

}
