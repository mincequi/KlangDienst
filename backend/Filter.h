#pragma once

#include <span>

#include <AudioChannel.h>
#include <Biquad.h>
#include <FilterParams.h>

class Filter {
public:
    Filter();

    bool isValid() const;
    void setFilterParams(const FilterParams& params);
    void setCascadeCount(uint8_t count);
    void setSampleRate(uint32_t rate);

private:
    friend class KlangDienstDsp;
    void onProcess(AudioChannel channel, const std::span<const float>& in, const std::span<float>& out);

    Biquad _biquad;
};
