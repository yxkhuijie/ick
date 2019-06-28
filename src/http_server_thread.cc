#include "src/http_server_thread.h"
#include "src/logger.h"

#include "evbuffer-internal.h"

HttpServerThread::HttpServerThread() {
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
}

void HttpServerThread::defaultGenericCallback(struct evhttp_request *req, void *arg) {
    const struct evhttp_uri* evhttp_uri = evhttp_request_get_evhttp_uri(req);
    char url[8192];
    evhttp_uri_join(const_cast<struct evhttp_uri*>(evhttp_uri), url, 8192);
    Logger::getInstance()->Info("receive request url: " + std::string(url));
    struct evbuffer* ev_buf = evbuffer_new();
    if (!ev_buf) {
        Logger::getInstance()->Error("create evbuffer failed!");
        return;
    }
    evbuffer_add_printf(ev_buf, "{'status':'OK'}");
    std::size_t total_len = ev_buf->total_len;
    std::string total_len_str = std::to_string(total_len);
    evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Length", total_len_str.c_str());
    evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "application/json");
    evhttp_send_reply(req, HTTP_OK, "OK", ev_buf);
    evbuffer_free(ev_buf);
}

void HttpServerThread::Start(evutil_socket_t listeningSocket) {

    this->listeningSocket = listeningSocket;
    if (evhttp_accept_socket(this->http, this->listeningSocket) < 0) {

        return;
    }

    evhttp_set_gencb(this->http, defaultGenericCallback, NULL);
    this->thread = std::thread([&] {

        Logger::getInstance()->Info("http server thread loop started");
        event_base_dispatch(this->base);
        Logger::getInstance()->Info("http server thread loop exited");

    });
    this->isRunning = true;
}

void HttpServerThread::Stop() {

    event_base_loopexit(this->base, nullptr);
    if (this->thread.joinable()) {
        this->thread.join();
    }
    this->isRunning = false;
}

HttpServerThread::~HttpServerThread() {
    Stop();
    event_base_free(this->base);
    evhttp_free(this->http);
}

evhttp* HttpServerThread::GetHttp() const {
    return this->http;
}

