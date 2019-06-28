#ifndef SRC_FILE_H_
#define SRC_FILE_H_

#include <list>
#include <string>

class File {
 public:
  // static bool readFile(const std::string& path, std::string* str);
  // static bool readFile(const std::string& path, std::vector<std::string>* vec);
  static bool readFile(const std::string& path, std::list<std::string>* lst);
  // static bool readFile(const std::string& path, Json::Value* val);

  static bool exist(const std::string& path);
};

#endif
