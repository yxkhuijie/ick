#include "src/http_request.h"

HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(struct evhttp_request* req) {
  this->req_ = req;
}

HttpRequest::~HttpRequest() {}

void HttpRequest::AddBufferOut(const char* data, size_t len) {
  evbuffer_add(evhttp_request_get_output_buffer(this->req_), data, len);
}

void HttpRequest::SendReply(int code) {
  evhttp_send_reply(this->req_, code, NULL, NULL);
}

void HttpRequest::AddHeader(const char* key, const char* value) {
  evhttp_add_header(evhttp_request_get_output_headers(this->req_), key, value);
}

const char* HttpRequest::GetBody() {
  evbuffer_add(evhttp_request_get_input_buffer(this->req_), "", 1);
  return (const char*) evbuffer_pullup(evhttp_request_get_input_buffer(this->req_), -1);
}

const char* HttpRequest::GetHeader(const char* key) {
  return evhttp_find_header(evhttp_request_get_input_headers(this->req_), key);
}

const char* HttpRequest::GetQueryValue(const char* key) {
  struct evkeyvalq query;
  evhttp_parse_query(evhttp_request_get_uri(this->req_), &query);
  return evhttp_find_header(&query, key);
}
