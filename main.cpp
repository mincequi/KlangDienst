#include <memory>

#include "common/FilterRepository.h"
#include "frontend/WebServer.h"
#include "frontend/WebSocketHandler.h"

class ExampleFilterRepository : public FilterRepository {
    std::vector<FilterParams> filterParams() const override {
        return _filterParams;
    }

    void setFilterParams(int8_t idx, const FilterParams& filter) override {
        if (idx < 0) return;
        if (idx >= _filterParams.size()) {
            _filterParams.resize(idx + 1);
            _filterParams[idx] = filter;
        }
    }

    void setFilterParams(const std::vector<FilterParams>& filters) override {
        _filterParams = filters;
    }

private:
    std::vector<FilterParams> _filterParams {
        FilterParams(FilterType::LowPass,   12, -2, 2),
        FilterParams(FilterType::HighPass,  13, -3, 3),
        FilterParams(FilterType::Peaking,   14, -4, 4),
        FilterParams(FilterType::LowShelf,  15, -5, 5),
        FilterParams(FilterType::HighShelf, 16, -6, 6)
    };
};

int main(int argc, char *argv[]) {
    ExampleFilterRepository filterRepository;
    auto webSocketHandler = std::make_shared<WebSocketHandler>(filterRepository);
    WebServer webServer(webSocketHandler);
    webServer.start();

    return 0;
}
