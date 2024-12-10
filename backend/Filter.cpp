#include "Filter.h"

Filter::Filter() {
}

bool Filter::isValid() const {
    return _biquad.isValid();
}

void Filter::setFilterParams(const FilterParams& params) {
    _biquad.setFilterParams(params);
}

void Filter::setCascadeCount(uint8_t count) {
    _biquad.setCascadeCount(count);
}

void Filter::setSampleRate(uint32_t rate) {
    _biquad.setSampleRate(rate);
}

void Filter::onProcess(AudioChannel channel, const std::span<const float>& in_, const std::span<float>& out) {
    _biquad.process(channel, in_, out);
}
