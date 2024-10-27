#pragma once

#include "NodeBase.h"

class AudioSink : public NodeBase {
public:
    static std::string mediaClass() {
        return "Audio/Sink";
    }
    static AudioSink fromNode(NodeBase& node) {
        return AudioSink(std::move(node));
    }

private:
    AudioSink(NodeBase&& node) : NodeBase(std::move(node)) {
    }
};
