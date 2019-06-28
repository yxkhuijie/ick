#include "src/http_callback.h"

HttpCallback::HttpCallback(std::string path, std::function<void(HttpRequest *, void*) > cb, void* arg)
    : path(path), cb(cb), arg(arg) {}

HttpCallback::~HttpCallback() {
}

void* HttpCallback::GetArg() const {
    return this->arg;
}

std::function<void(HttpRequest*, void*) > HttpCallback::GetCb() const {
    return this->cb;
}

std::string HttpCallback::GetPath() const {
    return this->path;
}