#include "src/file.h"

#include <iostream>
#include <fstream>
#include <cassert>
#ifdef __linux__
#include <unistd.h>
#endif

bool File::readFile(const std::string& path, std::list<std::string>* lst, int start_line /* = 0 */) {
  if (path.empty() || lst == nullptr || !exist(path)) return false;
  std::ifstream infile; 
  infile.open(path.data());
  assert(infile.is_open());
  std::string s;
  int line = 0;
  while(getline(infile, s)) {
    if (line < start_line) {
      line++;
      continue;
    }
    lst->push_back(s);
    line++;
  }
  infile.close();
  return true;
}

bool File::exist(const std::string& path) {
  if (path.empty()) return false;
  return (::access(path.c_str(), 0 )) != -1;
}