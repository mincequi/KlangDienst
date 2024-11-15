#pragma once

#include <map>
#include <span>
#include <vector>

#include <spa/pod/builder.h>

#include <pipewire/pipewire.h>
#include <pipewire/filter.h>

#include <rohrkabel/loop.hpp>

#include "AudioChannel.h"

namespace pw = pipewire;

class KlangDienstDsp {
public:
    KlangDienstDsp(std::shared_ptr<pw::main_loop> loop);
    ~KlangDienstDsp();

private:
    friend void on_process(void* userdata, struct spa_io_position* position);

    void onProcess(AudioChannel channel, const std::span<const float>& in, std::span<float>& out);

    struct port {
        KlangDienstDsp *data;
    };

    std::shared_ptr<pw::main_loop> _loop;
    pw_filter* _filter;
    const struct spa_pod* _params[1];
    uint8_t _buffer[1024];
    spa_pod_builder _b;

    std::map<AudioChannel, port*> _inPorts;
    std::map<AudioChannel, port*> _outPorts;
    std::vector<float> _silence;
};
