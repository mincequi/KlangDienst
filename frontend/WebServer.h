#pragma once

#include <functional>
#include <memory>

class WebServer {
public:
    WebServer();

    void start();

private:
    // pimpl
    class Impl;
    std::shared_ptr<Impl> _impl;
};
