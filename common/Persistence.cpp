#include "Persistence.h"

#include <map>
#include <string>

#include <rfl/toml.hpp>

using namespace std;

static const std::string _configFile = "/var/lib/KlangDienst/KlangDienst.toml";

vector<FilterParams> Persistence::filterParams() const {
    std::ifstream file(_configFile);
    if (!file) return {};

    ostringstream ss;
    ss << file.rdbuf();
    std::string str = ss.str();

    try {
        auto out = rfl::toml::read<map<string, vector<FilterParams>>>(str);
        if (out) {
            _filterParams = (*out)["filters"];
        }
    } catch (const std::exception& e) {
        return {};
    }
    return _filterParams;
}

void Persistence::setFilterParams(int8_t idx, const FilterParams& params) {
    if (idx >= _filterParams.size()) {
        _filterParams.resize(idx + 1);
    }
    _filterParams[idx] = params;

    std::ofstream file(_configFile);
    file << rfl::toml::write(map<string, vector<FilterParams>>{{"filters", _filterParams}});
}

void Persistence::setFilterParams(const vector<FilterParams>& params) {
    _filterParams = params;

    std::ofstream file(_configFile);
    file << rfl::toml::write(map<string, vector<FilterParams>>{{"filters", _filterParams}});
}
