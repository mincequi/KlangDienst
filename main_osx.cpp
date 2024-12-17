#include <iostream>

#include <magic_enum/magic_enum.hpp>

#include "common/FilterRepository.h"
#include "frontend/WebServer.h"
#include "frontend/WebSocketHandler.h"

using namespace std;

class Dsp : public FilterRepository {
    virtual std::vector<FilterParams> filterParams() const {
        return _filterParams;
    }

    // Set the filter parameters for a given filter index
    virtual void setFilterParams(int8_t idx, const FilterParams& params) {
        if (idx >= _filterParams.size()) {
            _filterParams.resize(idx + 1);
        }
        _filterParams[idx] = params;

        cout << idx << "> type: " << magic_enum::enum_name(params.type) << ", f: " << params.f() << ", g: " << params.g() << ", q: " << params.q() << endl;
    }

    // Set the filter parameters for all filters
    virtual void setFilterParams(const std::vector<FilterParams>& params) {
        _filterParams = params;
    }

    std::vector<FilterParams> _filterParams;
};

int main(int argc, char *argv[]) {
    Dsp dsp;
    auto webSocketHandler = std::make_shared<WebSocketHandler>(dsp);
    WebServer webServer(webSocketHandler);
    webServer.start();

    return 0;
}
