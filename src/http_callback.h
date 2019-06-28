#ifndef HTTPCALLBACK_H
#define HTTPCALLBACK_H

#include <functional>

#include "src/http_request.h"

class HttpCallback {
public:
    HttpCallback(std::string path, std::function<void(HttpRequest *, void*) > cb, void* arg);
    void* GetArg() const;
    std::function<void(HttpRequest*, void*) > GetCb() const;
    std::string GetPath() const;
    virtual ~HttpCallback();

private:
    std::string path;
    std::function<void(HttpRequest*, void*) > cb;
    void *arg;
};

#endif /* HTTPCALLBACK_H */