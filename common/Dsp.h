#pragma once

#include <functional>
#include <vector>

#include "FilterParams.h"

struct DspStats;

// An interface class for filter repositories
class Dsp {
public:
    virtual ~Dsp() = default;

    virtual std::vector<FilterParams> filterParams() const = 0;

    // Set the filter parameters for a given filter index
    virtual void setFilterParams(int8_t idx, const FilterParams& params) = 0;

    // Set the filter parameters for all filters
    virtual void setFilterParams(const std::vector<FilterParams>& params) = 0;

    inline void setStatsCallback(std::function<void(const DspStats&)> callback) {
        _callback = callback;
    }

protected:
    inline void notifyStats(const DspStats& stats) {
        if (_callback) {
            _callback(stats);
        }
    }

private:
    std::function<void(const DspStats&)> _callback;
};
