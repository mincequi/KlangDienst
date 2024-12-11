#pragma once

#include <optional>
#include <span>

#include "FilterType.h"

struct FilterParams {
    static std::optional<FilterParams> from(std::span<const int8_t> data);

    FilterParams() = default;
    FilterParams(FilterType type, int8_t fIdx, int8_t gIdx, int8_t qIdx);

    FilterType type = FilterType::Bypass;
    int8_t fIdx = 0;
    int8_t gIdx = 0;
    int8_t qIdx = 0;

    float f    = 0.0;
    float g    = 0.0;
    float q    = 0.0;

    inline bool isValid() const {
        return (type != FilterType::Bypass && f >= 20.0 && f <= 20000.0 && q > 0.0);
    }

    inline bool operator==(const FilterParams& other) {
        return type == other.type && f == other.f && g == other.g && q == other.q;
    }

    // assignment operator
    FilterParams& operator=(const FilterParams& other) {
        type = other.type;
        f = other.f;
        g = other.g;
        q = other.q;
        return *this;
    }
};
