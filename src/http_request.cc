#include "src/http_request.h"

#include <sstream>

#include "src/logger.h"
#include "http-internal.h"

HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(struct evhttp_request* req) {
  this->req_ = req;
  response_header_.insert(std::pair<std::string, std::string>(
      "Content-Type", "application/json;charset=utf-8"));
}

HttpRequest::~HttpRequest() {}

void HttpRequest::AddBufferOut(const char* data, size_t len) {
    evbuffer_add(evhttp_request_get_output_buffer(this->req_), data, len);
}

void HttpRequest::SendReply(int code /* = HTTP_OK */) {
    evhttp_send_reply(this->req_, code, NULL, NULL);
}

void HttpRequest::SendReply(const std::string& data, int code /* = HTTP_OK */) {
  for (const auto& it : response_header_) {
    evhttp_add_header(evhttp_request_get_output_headers(
        this->req_), it.first.c_str(), it.second.c_str());
  }
  evbuffer_add(evhttp_request_get_output_buffer(this->req_), data.c_str(), data.length());
  evhttp_send_reply(this->req_, code, NULL, NULL);
}

void HttpRequest::AddHeader(const char* key, const char* value) {
  auto it = response_header_.find(std::string(key));
  if (it == response_header_.end()) {
    response_header_.insert(std::pair<std::string, std::string>(key, value));
  } else {
    it->second = value;
  }
  // evhttp_add_header(evhttp_request_get_output_headers(this->req_), key, value);
}

std::string HttpRequest::CreateResponse(
    const std::string& content,
    int errorCode /* = 0 */,
    const std::string& errorMessage /* = "OK" */) {
  std::stringstream ss;
  ss << "{'code':" << errorCode
     << ",'status':'" << errorMessage << "'"
     << ",'result':'" << content << "'}";
  return ss.str();
}

std::string HttpRequest::getBody() {
    evbuffer_add(evhttp_request_get_input_buffer(this->req_), "", 1);
    return (const char*) evbuffer_pullup(evhttp_request_get_input_buffer(this->req_), -1);
}

std::string HttpRequest::getHeader(const char* key) {
    return evhttp_find_header(evhttp_request_get_input_headers(this->req_), key);
}

std::string HttpRequest::getQueryValue(const char* key) {
    struct evkeyvalq query;
    evhttp_parse_query(evhttp_request_get_uri(this->req_), &query);
    const char* res = evhttp_find_header(&query, key);
    return res == nullptr ? "" : std::string(res);
}

std::string HttpRequest::getRequestUri() {
    const struct evhttp_uri* evhttp_uri = evhttp_request_get_evhttp_uri(req_);
    char url[8192];
    evhttp_uri_join(const_cast<struct evhttp_uri*>(evhttp_uri), url, 8192);
    return std::string(url);
}

std::string HttpRequest::getRequestAddress() {
    const struct evhttp_connection* evhttp_conn = evhttp_request_get_connection(req_);
    if (evhttp_conn == nullptr) {
      return "";
    } else {
      return std::string(evhttp_conn->address) + ":" + std::to_string(evhttp_conn->port);
    }
}

std::string HttpRequest::getRequestHost() {
    const struct evhttp_connection* evhttp_conn = evhttp_request_get_connection(req_);
    if (evhttp_conn == nullptr) {
      return "";
    } else {
      return evhttp_conn->address;
    }
}

int32_t HttpRequest::getRequestPort() {
    const struct evhttp_connection* evhttp_conn = evhttp_request_get_connection(req_);
    if (evhttp_conn == nullptr) {
      return 0;
    } else {
      return evhttp_conn->port;
    }
}
