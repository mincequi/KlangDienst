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

    return FilterParams::from(type.value(), data[1], data[2], data[3]);
}

FilterParams FilterParams::from(FilterType type, int8_t fIdx, int8_t gIdx, int8_t qIdx) {
    FilterParams params;
    params.type = type;
    params.fIdx = fIdx;
    params.gIdx = gIdx;
    params.qIdx = qIdx;
    return params;
}

float FilterParams::q() const { return _baseQ / qIdx; }

