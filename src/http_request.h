#ifndef SRC_HTTP_REQUEST_H_
#define SRC_HTTP_REQUEST_H_

#include <evhttp.h>
#include <string>

class HttpServer;

class HttpRequest {
 public:
  HttpRequest();
  HttpRequest(struct evhttp_request* req);
  virtual ~HttpRequest();

  void AddBufferOut(const char* data, size_t len);
  void SendReply(int code);
  void AddHeader(const char* key, const char* value);
  
  std::string getHeader(const char* key);
  std::string getQueryValue(const char* key);
  std::string getBody();

  // get request uri
  std::string getRequestUri();
  // get request ip:port
  std::string getRequestAddress();
  // get request ip
  std::string getRequestHost();
  // get request port
  int32_t getRequestPort();

 public:
 private:
  struct evhttp_request* req_;
};

#endif