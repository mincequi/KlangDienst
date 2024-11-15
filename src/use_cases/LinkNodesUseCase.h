#pragma once

#include <magic_enum_utility.hpp>
#include <rohrkabel/link/link.hpp>

#include "AudioChannel.h"
#include "PortRegistry.h"

class LinkNodesUseCase {
public:
    LinkNodesUseCase(std::shared_ptr<pw::core> core,
                     const PortRegistry& portRegistry) :
        _core(core),
        _portRegistry(portRegistry) {
    }

    void operator()(pw::node& sourceNode,
                    pw::node& sinkNode) {
        magic_enum::enum_for_each<AudioChannel>([&](AudioChannel channel) {
            auto sourcePort = _portRegistry.port(sourceNode, pw::port_direction::output, channel);
            auto sinkPort = _portRegistry.port(sinkNode, pw::port_direction::input, channel);
            if (!sourcePort || !sinkPort) {
                warn("Failed to link nodes: source or sink port not found");
            } else {
                _links.emplace_back(*_core->create<pw::link>(pw::link_factory{sourcePort->id, sinkPort->id}).get());
            }
        });
    };

private:
    std::shared_ptr<pw::core> _core;
    const PortRegistry& _portRegistry;
    std::vector<pw::link> _links;
};
