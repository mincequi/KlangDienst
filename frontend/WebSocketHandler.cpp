#include "WebSocketHandler.h"

#include <FilterRepository.h>

WebSocketHandler::WebSocketHandler(FilterRepository& filterRepository) : _filterRepository(filterRepository) {}

void WebSocketHandler::handleNewMessage(const WebSocketConnectionPtr& conn, std::string&& message, const WebSocketMessageType& type) {
    if (type != WebSocketMessageType::Binary) {
        return;
    }
    if (message.size() == 5) {
        int8_t idx = message[0];
        auto subSpan = std::span<int8_t>(reinterpret_cast<int8_t*>(const_cast<char*>(message.data() + 1)), 4);
        auto filter = FilterParams::from(subSpan);
        if (filter.has_value()) {
            _filterRepository.setFilterParams(idx, filter.value());
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
        _filterRepository.setFilterParams(filters);
    }
}

void WebSocketHandler::handleNewConnection(const HttpRequestPtr& req, const WebSocketConnectionPtr& conn) {
    std::vector<char> data;
    data.reserve(_filterRepository.filterParams().size() * 4);
    for (const auto& filter : _filterRepository.filterParams()) {
        data.push_back(static_cast<char>(filter.type));
        data.push_back(filter.fIdx);
        data.push_back(filter.gIdx);
        data.push_back(filter.qIdx);
    }

    conn->send(data.data(), data.size(), WebSocketMessageType::Binary);
}

void WebSocketHandler::handleConnectionClosed(const WebSocketConnectionPtr& conn) {
    // Handle connection closed
}
