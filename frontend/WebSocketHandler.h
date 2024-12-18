#pragma once

#include <list>

#include <drogon/WebSocketController.h>

using namespace drogon;

class Dsp;
struct DspStats;

// A WebSocket handler for the frontend
//
// We need to set the second template parameter to false to disable auto creation
// since we do not offer a default constructor.
class WebSocketHandler : public WebSocketController<WebSocketHandler, false> {
public:
    WebSocketHandler(Dsp& dsp);

    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/ws");
    WS_PATH_LIST_END

    void send(const char* msg, uint64_t len);

private:
    void handleNewMessage(const WebSocketConnectionPtr&, std::string&&, const WebSocketMessageType&) override;
    void handleNewConnection(const HttpRequestPtr&, const WebSocketConnectionPtr&) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;

    void onStatsChanged(const DspStats& stats);

    Dsp& _dsp;
    std::list<WebSocketConnectionPtr> _connections;

    std::mutex _mutex;
};
