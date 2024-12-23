#pragma once

#include "FilterParams.h"

class Persistence {
public:
    Persistence() = default;

    std::vector<FilterParams> filterParams() const;

    // Set the filter parameters for a given filter index
    void setFilterParams(int8_t idx, const FilterParams& params);

    // Set the filter parameters for all filters
    void setFilterParams(const std::vector<FilterParams>& params);

private:
    mutable std::vector<FilterParams> _filterParams;
};
