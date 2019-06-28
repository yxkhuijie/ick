#ifndef SRC_HTTP_REQUEST_H_
#define SRC_HTTP_REQUEST_H_

#include <evhttp.h>

class HttpServer;

class HttpRequest {
 public:
  HttpRequest();
  HttpRequest(struct evhttp_request* req);
  virtual ~HttpRequest();

  void setHttpServer(HttpServer* http_server);
  void AddBufferOut(const char* data, size_t len);
  void SendReply(int code);
  void AddHeader(const char* key, const char* value);
  const char* GetHeader(const char* key);
  const char* GetQueryValue(const char* key);
  const char* GetBody();

 public:
  /* The client's socket. */
  int request_socket_fd_;
  /* The event_base for this client. */
  struct event_base *evbase_;
  /* The bufferedevent for this client. */
  struct bufferevent *buf_ev_;
  /* The output buffer for this client. */
  struct evbuffer *output_buffer_;
  /* Here you can add your own application-specific attributes which
   * are connection-specific. */
  HttpServer* http_server_;
 private:
  struct evhttp_request* req_;
};

#endif