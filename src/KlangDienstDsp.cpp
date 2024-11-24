#include "KlangDienstDsp.h"

#include <map>

#include <magic_enum/magic_enum_utility.hpp>
#include <spa/param/latency-utils.h>
#include <spa/param/latency.h>
#include <spdlog/spdlog.h>

using namespace std::placeholders;
using namespace spdlog;

std::vector<float> silence;

void on_process(void* userdata, struct spa_io_position* position) {
    KlangDienstDsp* node = (KlangDienstDsp*)userdata;

    auto sampleCount = position->clock.duration;
    auto sampleRate = position->clock.rate.denom;
    node->setSampleRate(sampleRate);

    magic_enum::enum_for_each<AudioChannel>([&](AudioChannel channel) {
        float* in = (float*)pw_filter_get_dsp_buffer(node->_inPorts[channel], sampleCount);
        float* out = (float*)pw_filter_get_dsp_buffer(node->_outPorts[channel], sampleCount);

        std::span<const float> inSpan;
        std::span<float> outSpan;
        if (in == nullptr || out == nullptr) {
            /*if (silence.size() != sampleCount) {
                silence.resize(sampleCount);
                for (size_t i = 0; i < sampleCount; i++) {
                    silence[i] = 0.0f;
                }
            }
            inSpan = std::span<const float>(silence);
            outSpan = std::span<float>(silence);
            std::cerr << "playing silence" << std::endl;*/
        } else {
            inSpan = std::span<const float>(in, sampleCount);
            outSpan = std::span<float>(out, sampleCount);
        }

        node->onProcess(channel, inSpan, outSpan);
    });
}

static const struct pw_filter_events filter_events = {
    .version = PW_VERSION_FILTER_EVENTS,
    .process = on_process,
};

KlangDienstDsp::KlangDienstDsp(std::shared_ptr<pw::main_loop> loop) : _loop(loop) {
    _filter = pw_filter_new_simple(
                loop->loop(),
                "KlangDienstDsp",
                pw_properties_new(
                    PW_KEY_MEDIA_TYPE, "Audio",
                    PW_KEY_MEDIA_CATEGORY, "Filter",
                    PW_KEY_MEDIA_ROLE, "DSP",
                    NULL),
                &filter_events,
                this);

    magic_enum::enum_for_each<AudioChannel>([&](AudioChannel channel) {
        _inPorts[channel] = (port*)pw_filter_add_port(_filter,
                                                      PW_DIRECTION_INPUT,
                                                      PW_FILTER_PORT_FLAG_MAP_BUFFERS,
                                                      sizeof(port),
                                                      pw_properties_new(
                                                          PW_KEY_FORMAT_DSP, "32 bit float mono audio",
                                                          PW_KEY_PORT_NAME, ("input_" + std::string(magic_enum::enum_name(channel).data())).data(),
                                                          PW_KEY_AUDIO_CHANNEL, magic_enum::enum_name(channel).data(),
                                                          NULL),
                                                      nullptr, 0);
        _outPorts[channel] = (port*)pw_filter_add_port(_filter,
                                                       PW_DIRECTION_OUTPUT,
                                                       PW_FILTER_PORT_FLAG_MAP_BUFFERS,
                                                       sizeof(port),
                                                       pw_properties_new(
                                                           PW_KEY_FORMAT_DSP, "32 bit float mono audio",
                                                           PW_KEY_PORT_NAME, ("output_" + std::string(magic_enum::enum_name(channel).data())).data(),
                                                           PW_KEY_AUDIO_CHANNEL, magic_enum::enum_name(channel).data(),
                                                           NULL),
                                                       nullptr, 0);
    });

    _b = SPA_POD_BUILDER_INIT(_buffer, sizeof(_buffer));
    spa_process_latency_info latency_info{};
    latency_info.ns = static_cast<uint64_t>(10 * SPA_NSEC_PER_MSEC);
    _params[0] = spa_process_latency_build(&_b,
                                           SPA_PARAM_ProcessLatency,
                                           &latency_info);
    if (pw_filter_connect(_filter, PW_FILTER_FLAG_NONE /*PW_FILTER_FLAG_RT_PROCESS*/, _params, 1) < 0) {
        fprintf(stderr, "can't connect\n");
    }
}

KlangDienstDsp::~KlangDienstDsp() {
    pw_filter_destroy(_filter);
}

void KlangDienstDsp::setFilterParams(uint8_t index, const FilterParams& filterParams) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (index >= _filters.size()) {
        _filters.resize(index + 1);
    }
    _filters[index].setFilterParams(filterParams);
}

void KlangDienstDsp::setSampleRate(uint32_t sampleRate) {
    if (_sampleRate == sampleRate) {
        return;
    }
    _sampleRate = sampleRate;
    info("sample rate changed to {}", _sampleRate);

    for (auto& filter : _filters) {
        filter.setSampleRate(sampleRate);
    }
}

void KlangDienstDsp::onProcess(AudioChannel channel, const std::span<const float>& in, const std::span<float>& out) {
    std::lock_guard<std::mutex> lock(_mutex);
    std::copy(in.begin(), in.end(), out.begin());

    for (auto& filter : _filters) {
        if (filter.isValid())
            filter.onProcess(channel, in, out);
    }
}
