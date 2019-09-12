#ifndef SRC_FILE_H_
#define SRC_FILE_H_

#include <list>
#include <string>
#include <vector>

class File {
 public:
  // static bool readFile(const std::string& path, std::string* str);
  // static bool readFile(const std::string& path, std::vector<std::string>* vec);
  static bool readFile(const std::string& path, std::list<std::string>* lst, int start_line = 0);
  // static bool readFile(const std::string& path, Json::Value* val);
  static bool ReadFile(const std::string& path, std::string* str);
  static bool ReadFileAsBinary(const std::string& path, std::string* str);
  static bool IsDir(const std::string& path);
  static bool GetFilesInDir(
      const std::string& path,
      std::vector<std::string>* res,
      bool sub_dir = false);
  static std::string GetWorkDir();
  static std::string GetFileSuffixName(const std::string& file_name);

  static bool exist(const std::string& path);
};

#endif
