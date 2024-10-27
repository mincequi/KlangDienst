#pragma once

#include "NodeBase.h"

class StreamOutputAudio : public NodeBase {
public:
    static std::string mediaClass() {
        return "Stream/Output/Audio";
    }
    static StreamOutputAudio fromNode(NodeBase& node)  {
        return StreamOutputAudio(std::move(node));
    }

private:
    StreamOutputAudio(NodeBase&& node) : NodeBase(std::move(node)) {
    }
};
