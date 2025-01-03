#pragma once

#include <memory>

class WebSocketHandler;

namespace cmrc {
class embedded_filesystem;
}

class WebServer {
public:
    WebServer(std::shared_ptr<WebSocketHandler> webSocketHandler);
    ~WebServer();

    void start();

private:
    std::shared_ptr<WebSocketHandler> _webSocketHandler;
    std::shared_ptr<cmrc::embedded_filesystem> _fs;
};
