#pragma once

#include <map>
#include <mutex>
#include <span>
#include <vector>

#include <spa/pod/builder.h>

#include <pipewire/pipewire.h>
#include <pipewire/filter.h>

#include <rohrkabel/loop.hpp>

#include "AudioChannel.h"
#include "Dsp.h"
#include "Filter.h"

namespace pw = pipewire;

class Persistence;

class KlangDienstDsp : public Dsp {
public:
    KlangDienstDsp(std::shared_ptr<pw::main_loop> loop, Persistence& persistence);
    ~KlangDienstDsp();

    virtual std::vector<FilterParams> filterParams() const override;

    // Set the filter parameters for a given filter index
    virtual void setFilterParams(int8_t idx, const FilterParams& params) override;

    // Set the filter parameters for all filters
    virtual void setFilterParams(const std::vector<FilterParams>& params) override;

private:
    void setSampleRate(uint32_t sampleRate);

    friend void on_process(void* userdata, struct spa_io_position* position);
    void onProcess(AudioChannel channel, const std::span<const float>& in, const std::span<float>& out);

    struct port {
        KlangDienstDsp *data;
    };

    std::shared_ptr<pw::main_loop> _loop;
    Persistence& _persistence;

    pw_filter* _filter;
    const struct spa_pod* _params[1];
    uint8_t _buffer[1024];
    spa_pod_builder _b;

    std::map<AudioChannel, port*> _inPorts;
    std::map<AudioChannel, port*> _outPorts;
    std::vector<float> _silence;

    uint32_t _sampleRate = 0;
    std::vector<Filter> _filters;

    float _levelDb = 0.0f;
    uint32_t _sampleCount = 0;

    mutable std::mutex _mutex;
};
