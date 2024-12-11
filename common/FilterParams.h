#pragma once

#include <optional>
#include <span>

#include "FilterType.h"
#include "FrequencyTable.h"

struct FilterParams {
    static std::optional<FilterParams> from(std::span<const int8_t> data);
    static FilterParams from(FilterType type, int8_t fIdx, int8_t gIdx, int8_t qIdx);

    FilterParams() = default;

    FilterType type = FilterType::Bypass;
    int8_t fIdx = 0;
    int8_t gIdx = 0;
    int8_t qIdx = 0;

    inline float f() const { return frequencyTable[fIdx]; }
    inline float g() const { return gIdx * 0.5; }
    float q() const;

    inline bool isValid() const {
        return (type != FilterType::Bypass && fIdx >= 0 && fIdx <= 127 && qIdx > 0);
    }

    inline bool operator==(const FilterParams& other) {
        return type == other.type &&
               fIdx == other.fIdx &&
               gIdx == other.gIdx &&
               qIdx == other.qIdx;
    }

    // assignment operator
    FilterParams& operator=(const FilterParams& other) {
        type = other.type;
        fIdx = other.fIdx;
        gIdx = other.gIdx;
        qIdx = other.qIdx;
        return *this;
    }
};
