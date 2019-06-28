#ifndef HTTPSERVERTHREAD_H
#define HTTPSERVERTHREAD_H

#include <atomic>
#include <evhttp.h>
#include <thread>

class HttpServerThread {
public:

    HttpServerThread();
    virtual ~HttpServerThread();
    evhttp* GetHttp() const;

    void Start(evutil_socket_t listeningSocket);
    void Stop();

private:
    static void defaultGenericCallback(struct evhttp_request *req, void *arg);

    std::thread thread;
    std::atomic<bool> isRunning{false};

    struct event_base *base = nullptr;
    struct evhttp *http = nullptr;

    evutil_socket_t listeningSocket = -1;

};

#endif // HTTPSERVERTHREAD_H