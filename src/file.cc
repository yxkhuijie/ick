#include "src/file.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cassert>
#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

#include "src/logger.h"

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

bool File::ReadFile(const std::string& path, std::string* str) {
  if (path.empty() || !exist(path) || str == nullptr) return false;
  std::ifstream infile; 
  infile.open(path.data());
  assert(infile.is_open());
  std::stringstream ss;
  std::string s;
  while (getline(infile, s)) {
    ss << s << "\n";
  }
  infile.close();
  if (!ss.str().empty()) {
    str->resize(ss.str().length());
    char* dst = const_cast<char*>(str->data());
    memcpy(dst, ss.str().data(), ss.str().length());
  }
  return true;
}

bool File::ReadFileAsBinary(const std::string& path, std::string* str) {
  if (path.empty() || !exist(path) || str == nullptr) return false;
  std::ifstream infile; 
  infile.open(path.data(), std::ios::binary);
  assert(infile.is_open());
  infile.seekg(0, std::ios::end);
  std::size_t file_size = infile.tellg();
  if (file_size > 0) {
    str->resize(file_size);
    infile.seekg(0, std::ios::beg);
    char* buffer = const_cast<char*>(str->data());
    infile.read(buffer, file_size);
  }
  infile.close();
  return true;
}

bool File::IsDir(const std::string& path) {
  if (path.empty()) return false;
  std::string file_path = path;
#ifdef __linux__
  if (file_path[0] != '/') {
    file_path = GetWorkDir() + "/" + path;
  }
  struct stat st;
  stat(file_path.c_str(), &st);
  return S_ISDIR(st.st_mode);
#elif __windows__
  return false;
#endif
}

bool File::GetFilesInDir(
    const std::string& path,
    std::vector<std::string>* res,
    bool sub_dir) {
  if (path.empty() || res == nullptr) return false;
  std::string file_path = path;
#ifdef __linux__
  if (file_path[0] != '/') {
    file_path = GetWorkDir() + "/" + path;
  }
  struct dirent *dirp;
  DIR* dir = opendir(file_path.c_str());
  if (dir == nullptr) return false;  
  while ((dirp = readdir(dir)) != nullptr) {
    std::string d_name = dirp->d_name;
    if (d_name == "." || d_name == "..") continue;
    if (dirp->d_type == DT_REG) {  // file
      res->push_back(path.back() == '/' ? path + d_name : path + "/" + d_name);
    } else if (sub_dir && dirp->d_type == DT_DIR) {  // dir
      GetFilesInDir(path + "/" + d_name, res);
    }
  }  
  closedir(dir);
  return true;
#elif __windows__
  return false;
#endif
}

std::string File::GetWorkDir() {
#ifdef __linux__
  char buffer[MAX_PATH];
  getcwd(buffer, MAX_PATH);
  return std::string(buffer);
#elif __windows__
  return "";
#endif
}

std::string File::GetFileSuffixName(const std::string& file_name) {
  if (file_name.empty()) return "";
  std::size_t index = file_name.find_last_of(".");
  if (index == std::string::npos) return "";
  return file_name.substr(index);
}

bool File::exist(const std::string& path) {
  if (path.empty()) return false;
  return (::access(path.c_str(), 0 )) != -1;
}