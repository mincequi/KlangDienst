#pragma once

#include <pipewire/keys.h>
#include <pipewire/node.h>
#include <pipewire/type.h>

#include <rohrkabel/core/core.hpp>
#include <rohrkabel/node/node.hpp>

namespace pw = pipewire;

class KlangDienstSink {
public:
    KlangDienstSink(pw::core& core) {


        auto props = pw::properties::create({ {PW_KEY_APP_ID, "KlangDienst"},
                                              {PW_KEY_NODE_NAME, "KlangDienstSink"},
                                              {PW_KEY_NODE_DESCRIPTION, "KlangDienstSink"},
                                              {PW_KEY_NODE_VIRTUAL, "true"},
                                              {PW_KEY_NODE_PASSIVE, "out"},
                                              {PW_KEY_FACTORY_NAME, "support.null-audio-sink"},
                                              {PW_KEY_MEDIA_CLASS, "Audio/Sink"},
                                              {"audio.position", "FL,FR"},
                                              {"monitor.channel-volumes", "false"},
                                              {"monitor.passthrough", "true"},
                                              {PW_KEY_PRIORITY_SESSION, "0"}
                                            });

        _node = static_cast<pw_proxy*>(
            pw_core_create_object(core,
                                  "adapter",
                                  PW_TYPE_INTERFACE_Node,
                                  PW_VERSION_NODE,
                                  &props.get()->dict,
                                  0));
    }
    ~KlangDienstSink() {
        pw_proxy_destroy(_node);
    }

private:
    pw_proxy* _node = nullptr;
};
