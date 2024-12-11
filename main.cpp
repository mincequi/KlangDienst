#include <iostream>
#include <memory>

#include <magic_enum/magic_enum.hpp>

#include "common/FilterRepository.h"
#include "frontend/WebServer.h"
#include "frontend/WebSocketHandler.h"

class ExampleFilterRepository : public FilterRepository {
    std::vector<FilterParams> filterParams() const override {
        return _filterParams;
    }

    void setFilterParams(int8_t idx, const FilterParams& filter) override {
        if (idx < 0) return;
        if (idx >= _filterParams.size()) _filterParams.resize(idx + 1);
        _filterParams[idx] = filter;
        std::cout << "updated filter> type: " << magic_enum::enum_name(filter.type)
                  << ", f: " << filter.f()
                  << ", g: " << filter.g()
                  << ", q: " << filter.q() << std::endl;
    }

    void setFilterParams(const std::vector<FilterParams>& filters) override {
        _filterParams = filters;
        for (int i = 0; i < filters.size(); i++) {
            std::cout << "updated filter> type: " << magic_enum::enum_name(filters[i].type)
                      << ", f: " << filters[i].f()
                      << ", g: " << filters[i].g()
                      << ", q: " << filters[i].q() << std::endl;
        }
    }

private:
    std::vector<FilterParams> _filterParams {
        FilterParams::from(FilterType::LowPass,   12, -2, 2),
        FilterParams::from(FilterType::HighPass,  13, -3, 3),
        FilterParams::from(FilterType::Peaking,   14, -4, 4),
        FilterParams::from(FilterType::LowShelf,  15, -5, 5),
        FilterParams::from(FilterType::HighShelf, 16, -6, 6)
    };
};

int main(int argc, char *argv[]) {
    ExampleFilterRepository filterRepository;
    auto webSocketHandler = std::make_shared<WebSocketHandler>(filterRepository);
    WebServer webServer(webSocketHandler);
    webServer.start();

    return 0;
}
