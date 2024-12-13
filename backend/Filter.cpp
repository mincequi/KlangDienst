#include "Filter.h"

#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>

using namespace spdlog;

Filter::Filter() {
}

bool Filter::isValid() const {
    return _biquad.isValid();
}

FilterParams Filter::filterParams() const {
    return _biquad.filterParams();
}

void Filter::setFilterParams(const FilterParams& params) {
    info("updated filter> type: {}, f: {}, g: {}, q: {},", magic_enum::enum_name(params.type), params.f(), params.g(), params.q());
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
