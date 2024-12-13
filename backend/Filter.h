#pragma once

#include <span>

#include <AudioChannel.h>
#include <Biquad.h>
#include <FilterParams.h>

/**
 * @brief A filter.
 *
 * A filter is a wrapper around one or more biquads.
 */
class Filter {
public:
    Filter();

    bool isValid() const;
    FilterParams filterParams() const;
    void setFilterParams(const FilterParams& params);
    void setCascadeCount(uint8_t count);
    void setSampleRate(uint32_t rate);

private:
    friend class KlangDienstDsp;
    void onProcess(AudioChannel channel, const std::span<const float>& in, const std::span<float>& out);

    // TODO: can be more than one
    Biquad _biquad;
};
