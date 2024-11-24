#pragma once

struct FilterParams {
    enum class Type {
        Invalid     = 0,

        // Linear filters
        Peak        = 1,
        LowPass,
        HighPass,
        LowShelf,
        HighShelf,
        AllPass,

        Crossover
    };

    Type  type = Type::Invalid;
    float f    = 0.0;
    float g    = 0.0;
    float q    = 0.0;

    inline bool isValid() const {
        return (type != Type::Invalid && f >= 20.0 && f <= 20000.0 && q > 0.0);
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
