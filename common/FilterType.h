#pragma once

#include <cstdint>

enum class FilterType : int8_t {
    Bypass  = 0,

    Gain,

    // Linear filters
    Peaking,
    LowPass,
    HighPass,
    LowShelf,
    HighShelf,

    AllPass,

    Loudness,
};
