#pragma once

#include <map>
#include <span>
#include <vector>

#include <spa/pod/builder.h>

#include <pipewire/pipewire.h>
#include <pipewire/filter.h>

#include <rohrkabel/loop.hpp>

namespace pw = pipewire;

class EqualizerNode {
public:
    EqualizerNode(std::shared_ptr<pw::main_loop> loop);
    ~EqualizerNode();

private:
    enum class Channel {
        FL,
        FR,
    };

    friend void on_process(void* userdata, struct spa_io_position* position);

    void onProcess(Channel channel, const std::span<const float>& in, std::span<float>& out);

    struct port {
        EqualizerNode *data;
    };

    std::shared_ptr<pw::main_loop> _loop;
    pw_filter* _filter;
    const struct spa_pod* _params[1];
    uint8_t _buffer[1024];
    spa_pod_builder _b;

    std::map<Channel, port*> _inPorts;
    std::map<Channel, port*> _outPorts;
    std::vector<float> _silence;
};
