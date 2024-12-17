#include "Filter.h"

#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>

using namespace spdlog;

Filter::Filter() {
}

bool Filter::isValid() const {
    for (const auto& b : _biquads) {
        if (!b.isValid()) {
            return false;
        }
    }
    return true;
}

FilterParams Filter::filterParams() const {
    return _filterParams;
}

void Filter::setFilterParams(const FilterParams& params) {
    if (_filterParams == params) {
        return;
    }

    info("updated filter> type: {}, f: {}, g: {}, q: {},", magic_enum::enum_name(params.type), params.f(), params.g(), params.q());
    _filterParams = params;

    switch (params.type) {
    case FilterType::Loudness:
        //f1: 40, g1: 11, q1: 0.863695,
        //f2: 100, g2: 11, q2: 0.308462
        //f1: 12500, g1: 8, q1: 1.43949
        _biquads.resize(4);
        _biquads[0].setFilterParams(FilterParams::from(FilterType::Gain,      1, -18.53 * params.gIdx / 20.0, params.qIdx));
        _biquads[1].setFilterParams(FilterParams::from(FilterType::Peaking,  16,   11.0 * params.gIdx / 20.0, 10));
        _biquads[2].setFilterParams(FilterParams::from(FilterType::Peaking,  32,   11.0 * params.gIdx / 20.0, 28));
        _biquads[3].setFilterParams(FilterParams::from(FilterType::Peaking, 116,    8.0 * params.gIdx / 20.0,  6));
        _biquads[0].setSampleRate(_sampleRate);
        _biquads[1].setSampleRate(_sampleRate);
        _biquads[2].setSampleRate(_sampleRate);
        _biquads[3].setSampleRate(_sampleRate);
        for (const auto& b : _biquads) {
            // log coeffs
            //info("a0: 1.0, a1: {}, a2: {}, b0: {}, b1: {}, b2: {}", b.coeffs().a1, b.coeffs().a2, b.coeffs().b0, b.coeffs().b1, b.coeffs().b2);
        }
        break;
    default:
        _biquads.resize(1);
        _biquads.front().setFilterParams(params);
        break;
    }
}

//void Filter::setCascadeCount(uint8_t count) {
//    _biquads.setCascadeCount(count);
//}

void Filter::setSampleRate(uint32_t rate) {
    _sampleRate = rate;
    for (auto& b : _biquads) {
        b.setSampleRate(rate);
    }
}

void Filter::onProcess(AudioChannel channel, const std::span<const float>& in_, const std::span<float>& out) {
    for (auto& b : _biquads) {
        b.process(channel, in_, out);
    }
}
