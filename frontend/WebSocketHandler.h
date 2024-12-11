#pragma once

#include <drogon/WebSocketController.h>

using namespace drogon;

class FilterRepository;

// A WebSocket handler for the frontend
//
// We need to set the second template parameter to false to disable auto creation
// since we do not offer a default constructor.
class WebSocketHandler : public WebSocketController<WebSocketHandler, false> {
public:
    WebSocketHandler(FilterRepository& filterRepository);

    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/ws");
    WS_PATH_LIST_END

private:
    void handleNewMessage(const WebSocketConnectionPtr&, std::string&&, const WebSocketMessageType&) override;
    void handleNewConnection(const HttpRequestPtr&, const WebSocketConnectionPtr&) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;

    FilterRepository& _filterRepository;
};
