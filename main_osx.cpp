#include <iostream>

#include <rfl/enums.hpp>

#include "common/Dsp.h"
#include "common/Persistence.h"
#include "frontend/WebServer.h"
#include "frontend/WebSocketHandler.h"

using namespace std;

class ExampleDsp : public Dsp {
public:
    ExampleDsp(Persistence& persistence) : _persistence(persistence) {
        _filterParams = _persistence.filterParams();
    }

    virtual std::vector<FilterParams> filterParams() const {
        return _filterParams;
    }

    // Set the filter parameters for a given filter index
    virtual void setFilterParams(int8_t idx, const FilterParams& params) {
        if (idx >= _filterParams.size()) {
            _filterParams.resize(idx + 1);
        }
        _filterParams[idx] = params;

        cout << idx << "> type: " << rfl::enum_to_string(params.type) << ", f: " << params.f() << ", g: " << params.g() << ", q: " << params.q() << endl;

        _persistence.setFilterParams(idx, params);
    }

    // Set the filter parameters for all filters
    virtual void setFilterParams(const std::vector<FilterParams>& params) {
        _filterParams = params;

        _persistence.setFilterParams(params);
    }

    Persistence& _persistence;
    std::vector<FilterParams> _filterParams;
};

int main(int argc, char *argv[]) {
    Persistence persistence;
    ExampleDsp dsp(persistence);
    auto webSocketHandler = std::make_shared<WebSocketHandler>(dsp);
    WebServer webServer(webSocketHandler);
    webServer.start();

    return 0;
}
