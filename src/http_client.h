#ifndef SRC_CLIENT_HTTP_CLIENT_H_
#define SRC_CLIENT_HTTP_CLIENT_H_

#include <string>
#include <map>
#include <functional>

class HttpClient {
 public:
  HttpClient();
  ~HttpClient();

  bool FetchUrl(const std::string& url, const std::string& data = std::string(""));
  const std::string& GetResponseBody();

  void SetHeader(const std::string& key, const std::string& value);
  // GET, POST
  void SetMethod(const std::string& http_method);
  void SetConnectTimeout(int timeout);
  void SetFetchTimeout(int timeout);
  int GetResponseCode();
  int GetCurlCode();

 private:
  static size_t OnWriteData(
      void* buffer, size_t size, size_t nmemb, void* lpVoid);

 private:
  std::map<std::string, std::string> request_header_map_;
  std::string response_body_;
  std::string http_method_;
  int connect_timeout_;
  int fetch_timeout_;
  int response_code_;
  int curl_code_;
};

#endif  // SRC_CLIENT_HTTP_CLIENT_H_
