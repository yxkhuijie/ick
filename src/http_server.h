#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <evhttp.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <atomic>
#include "src/http_server_thread.h"
#include "src/http_callback.h"

class HttpServer {
 public:
  HttpServer(std::string host = "0.0.0.0", uint16_t port = 8080, int threadNum = 1);
  HttpServer(const HttpServer& orig);
  virtual ~HttpServer();

  void Start();
  void Stop();

  void SetListeningPort(uint16_t port);
  void SetCallback(std::string path, std::function<void(HttpRequest *, void*) > cb, void *arg);
  void SetGenericCallback(std::function<void(HttpRequest *, void*) > cb, void *arg);

 private:
  static void callbackHandler(struct evhttp_request *req, void *arg);
  struct event_base *base = nullptr;
  struct evhttp *http = nullptr;
  struct evhttp_bound_socket *handle = nullptr;

  std::vector<HttpServerThread*> serverThreads;
  ev_uint16_t port = 0;
  std::string host;
  int threadNum;
  std::map<std::string, HttpCallback*> callbacks;
  HttpCallback *genericCallback { nullptr };
  evutil_socket_t listeningSocket = -1;
  std::atomic<bool> isRunning{false};
};

#endif /* HTTPSERVER_H */
