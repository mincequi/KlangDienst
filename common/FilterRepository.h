#pragma once

#include <vector>

#include "FilterParams.h"

// An interface class for filter repositories
class FilterRepository {
public:
    virtual ~FilterRepository() = default;

    virtual std::vector<FilterParams> filterParams() const = 0;

    // Set the filter parameters for a given filter index
    virtual void setFilterParams(int8_t idx, const FilterParams& params) = 0;

    // Set the filter parameters for all filters
    virtual void setFilterParams(const std::vector<FilterParams>& params) = 0;
};
