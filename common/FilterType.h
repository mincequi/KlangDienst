#pragma once

#include <cstdint>

enum class FilterType : int8_t {
    Bypass  = 0,

    // Linear filters
    Peaking     = 1,
    LowPass,
    HighPass,
    LowShelf,
    HighShelf,
    AllPass,

    Loudness    = 16,
};
