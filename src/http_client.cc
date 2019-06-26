#include "http_client.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include <curl/curl.h>
#include "src/logger.h"

HttpClient::HttpClient()
    : http_method_("POST"),
      connect_timeout_(3000),
      fetch_timeout_(3000),
      response_code_(200),
      curl_code_(CURLE_OK) {
  this->SetHeader("Accept", "*/*");
  this->SetHeader("Accept-Language", "zh-cn");
  this->SetHeader("User-Agent", "ICK CORE 2.0");
  this->SetHeader("Cache-Control", "no-cache");
  this->SetHeader("Content-Type", "application/json");
}

HttpClient::~HttpClient() {}

size_t HttpClient::OnWriteData(
    void* buffer, size_t size, size_t nmemb, void* lpVoid) {
  std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
  if( NULL == str || NULL == buffer ) {
    Logger::getInstance()->Error("http client on write data error!");
    return -1;
  }

  char* pData = (char*)buffer;
  str->append(pData, size * nmemb);
  return nmemb;
}

bool HttpClient::FetchUrl(const std::string& url, const std::string& data) {
  if (url.empty()) return false;
  Logger::getInstance()->Info("fetch url: " + url);
  CURLcode res;
  CURL* curl = curl_easy_init();
  if (curl == nullptr) {
    return false;
  }
  if (http_method_ == "POST") {
    SetHeader("Content-Length", std::to_string(data.length()));
  }
  struct curl_slist *headers = NULL;
  std::stringstream ss;
  for (const auto& it : request_header_map_) {
    std::string str = it.first + ":" + it.second;
    ss << str << "  ";
    headers = curl_slist_append(headers, str.c_str());
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  if (http_method_ == "GET") {
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
  } else if (http_method_ == "POST") {
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());
  }
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &HttpClient::OnWriteData);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response_body_);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout_ / 1000);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, fetch_timeout_ / 1000);

  res = curl_easy_perform(curl);
  Logger::getInstance()->Info("response body: " + response_body_);
  curl_easy_cleanup(curl);

  if (res != CURLE_OK) {
    switch (res) {
      case CURLE_UNSUPPORTED_PROTOCOL:
        Logger::getInstance()->Error("unsupport protocol error!");
        break;
      case CURLE_COULDNT_CONNECT:
        Logger::getInstance()->Error("connect failure error!");
        break;
      case CURLE_HTTP_RETURNED_ERROR:
        Logger::getInstance()->Error("http returned error!");
        break;
      case CURLE_READ_ERROR:
        Logger::getInstance()->Error("read local file error!");
        break;
      default:
        Logger::getInstance()->Error("error code: " + std::to_string(res));
        break;
    }
    response_code_ = res;
  } else {
    response_code_ = 200;
  }
  curl_code_ = res;
  return res == CURLE_OK;
}

void HttpClient::SetHeader(
    const std::string& key, const std::string& value) {
  std::map<std::string, std::string>::iterator it;
  if ((it = request_header_map_.find(key)) == request_header_map_.end()) {
    request_header_map_.insert(
        std::pair<std::string, std::string>(key, value));
  } else {
    it->second = value;
  }
}

void HttpClient::SetMethod(const std::string& http_method) {
  if (http_method.compare("GET") == 0 ||
      http_method.compare("POST") == 0) {
    http_method_ = http_method;
  } else {
    Logger::getInstance()->Error("http method not supported: " + http_method);
  }
}

void HttpClient::SetConnectTimeout(int timeout) {
  connect_timeout_ = timeout;
}

void HttpClient::SetFetchTimeout(int timeout) {
  fetch_timeout_ = timeout;
}

int HttpClient::GetResponseCode() {
  return response_code_;
}

int HttpClient::GetCurlCode() {
  return curl_code_;
}

const std::string& HttpClient::GetResponseBody() {
  return response_body_;
}
