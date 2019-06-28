#include <atomic>

#include "src/http_server.h"
#include "src/http_server_thread.h"

#include <event2/thread.h>

HttpServer::HttpServer(std::string host, uint16_t port, int threadNum) {
    this->host = host;
    this->port = port;
    this->threadNum = threadNum;

    evthread_use_pthreads();
    this->base = event_base_new();
    if (!this->base) {
        //TODO exception
        return;
    }
    this->http = evhttp_new(this->base);
    if (!this->http) {
        //TODO exception
        return;
    }

    for (int i = 0; i < this->threadNum; i++) {
        HttpServerThread *serverThread = new HttpServerThread();
        this->serverThreads.push_back(serverThread);
    }
}

HttpServer::HttpServer(const HttpServer& orig) {
}

void HttpServer::Stop() {
    if (!isRunning) {
        return;
    }
    for (HttpServerThread* it : this->serverThreads) {
        it->Stop();
    }
    evhttp_del_accept_socket(this->http, this->handle);
    isRunning = false;
}

HttpServer::~HttpServer() {

    Stop();
    for (auto it : this->serverThreads) {
        delete it;
    }
    for (std::map<std::string, HttpCallback*>::iterator it = this->callbacks.begin(); it != this->callbacks.end(); ++it) {
        delete it->second;
    }
    if (this->genericCallback) {
        delete this->genericCallback;
    }

    event_base_free(this->base);
    evhttp_free(this->http);
    libevent_global_shutdown();

}

void HttpServer::callbackHandler(struct evhttp_request *req, void *arg) {

    HttpServer *pServer = (HttpServer*) arg;
    std::map<std::string, HttpCallback*>::iterator it = pServer->callbacks.find(
        evhttp_uri_get_path(evhttp_request_get_evhttp_uri(req)));
    if (it != pServer->callbacks.end()) { //if exist

        std::function<void(HttpRequest*, void*) > cb = it->second->GetCb();

        HttpRequest *request = new HttpRequest(req);
        cb(request, it->second->GetArg());
        delete request;

    } else { //if not exist

        std::function<void(HttpRequest*, void*) > cb = pServer->genericCallback->GetCb();
        HttpRequest *request = new HttpRequest(req);
        cb(request, pServer->genericCallback->GetArg());
        delete request;
    }
}

void HttpServer::SetCallback(std::string path, std::function<void(HttpRequest*, void*) > cb, void* arg) {
    if (this->callbacks.find(path.c_str()) == this->callbacks.end()) { //if not exist
        HttpCallback *callback = new HttpCallback(path, cb, arg);
        this->callbacks.insert(std::make_pair(path, callback));

        for (HttpServerThread* it : this->serverThreads) {
            evhttp_set_cb(it->GetHttp(), path.c_str(), callbackHandler, (void*) this);
        }

    } else { //if exist
        //TODO exception
        return;
    }
}

void HttpServer::SetGenericCallback(std::function<void(HttpRequest*, void*) > cb, void* arg) {
    if (!this->genericCallback) {//if exist
        this->genericCallback = new HttpCallback("", cb, arg);

        for (HttpServerThread* it : this->serverThreads) {
            evhttp_set_gencb(it->GetHttp(), callbackHandler, (void*) this);
        }

    } else { //if not exist
        //TODO exception
        return;
    }

}

void HttpServer::Start() {

    if (isRunning.load()) {
        return;
    }

    this->handle = evhttp_bind_socket_with_handle(this->http, this->host.c_str(), this->port);
    if (!this->handle) {
        //TODO exception
        return;
    }

    this->listeningSocket = evhttp_bound_socket_get_fd(this->handle);

    if (listeningSocket == -1) {
        //TODO exception
        return;
    }

    for (HttpServerThread* it : this->serverThreads) {
        it->Start(this->listeningSocket);
    }
    isRunning = true;
}
