#include "FilterParams.h"

#include <cmath>
#include <magic_enum/magic_enum.hpp>

#include <FrequencyTable.h>

static double _fractionalStep = pow(2, 1.0 / 6.0);
static double _baseQ = sqrt(_fractionalStep) / (_fractionalStep - 1);

std::optional<FilterParams> FilterParams::from(std::span<const int8_t> data) {
    if (data.size() != 4) return std::nullopt;
    auto type = magic_enum::enum_cast<FilterType>(data[0]);
    if (!type) return std::nullopt;
    if (data[1] < 0 || data[1] >= frequencyTable.size()) return std::nullopt;

    FilterParams params(*type, data[1], data[2], data[3]);
    params.f = frequencyTable[data[1]];
    params.g = data[2] * 0.5;
    params.q = _baseQ / data[3];
    return params;
}

FilterParams::FilterParams(FilterType type, int8_t fIdx, int8_t gIdx, int8_t qIdx) : type(type), fIdx(fIdx), gIdx(gIdx), qIdx(qIdx), f(0.0), g(0.0), q(0.0) {}
