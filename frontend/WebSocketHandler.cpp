#include "WebSocketHandler.h"

#include <Dsp.h>
#include <DspStats.h>

WebSocketHandler::WebSocketHandler(Dsp& dsp) : _dsp(dsp) {}

void WebSocketHandler::send(const char *msg, uint64_t len) {
    std::lock_guard<std::mutex> lock(_mutex);
    for (const auto& conn : _connections) {
        conn->send(msg, len, WebSocketMessageType::Binary);
    }
}

void WebSocketHandler::handleNewMessage(const WebSocketConnectionPtr& conn, std::string&& message, const WebSocketMessageType& type) {
    if (type != WebSocketMessageType::Binary) {
        return;
    }
    if (message.size() == 5) {
        int8_t idx = message[0];
        auto subSpan = std::span<int8_t>(reinterpret_cast<int8_t*>(const_cast<char*>(message.data() + 1)), 4);
        auto filter = FilterParams::from(subSpan);
        if (filter.has_value()) {
            _dsp.setFilterParams(idx, filter.value());
        }
        return;
    }
    if (message.size()%4 == 0) {
        std::vector<FilterParams> filters;
        for (int i = 0; i < message.size(); i += 4) {
            auto subSpan = std::span<int8_t>(reinterpret_cast<int8_t*>(const_cast<char*>(message.data() + i)), 4);
            auto filter = FilterParams::from(subSpan);
            if (filter.has_value()) {
                filters.push_back(filter.value());
            }
        }
        _dsp.setFilterParams(filters);
    }
}

void WebSocketHandler::handleNewConnection(const HttpRequestPtr& req, const WebSocketConnectionPtr& conn) {
    std::vector<char> data;
    data.reserve(_dsp.filterParams().size() * 4);
    for (const auto& filter : _dsp.filterParams()) {
        data.push_back(static_cast<char>(filter.type));
        data.push_back(filter.fIdx);
        data.push_back(filter.gIdx);
        data.push_back(filter.qIdx);
    }

    {
        std::lock_guard<std::mutex> lock(_mutex);
        _connections.push_back(conn);
        conn->send(data.data(), data.size(), WebSocketMessageType::Binary);
    }
}

void WebSocketHandler::handleConnectionClosed(const WebSocketConnectionPtr& conn) {
    std::lock_guard<std::mutex> lock(_mutex);
    _connections.remove(conn);
}

void WebSocketHandler::onStatsChanged(const DspStats& stats) {
    send(reinterpret_cast<const char*>(&stats.levelDb), 1);
}
