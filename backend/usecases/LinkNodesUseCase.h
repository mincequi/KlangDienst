#pragma once

#include <magic_enum/magic_enum_utility.hpp>
#include <rohrkabel/link/link.hpp>

#include "AudioChannel.h"
#include "registries/PortRegistry.h"

class LinkNodesUseCase {
public:
    LinkNodesUseCase(std::shared_ptr<pw::core> core,
                     const PortRegistry& portRegistry) :
        _core(core),
        _portRegistry(portRegistry) {
    }

    void operator()(std::uint32_t sourceNodeId,
                    std::uint32_t sinkNodeId) const {
        magic_enum::enum_for_each<AudioChannel>([&](AudioChannel channel) {
            auto sourcePort = _portRegistry.port(sourceNodeId, pw::port_direction::output, channel);
            auto sinkPort = _portRegistry.port(sinkNodeId, pw::port_direction::input, channel);
            if (!sourcePort || !sinkPort) {
                warn("Failed to link nodes: source or sink port not found");
            } else {
                _links.emplace_back(*_core->create<pw::link>(pw::link_factory{ .input = sinkPort->id, .output = sourcePort->id }).get());
            }
        });
    };

private:
    std::shared_ptr<pw::core> _core;
    const PortRegistry& _portRegistry;
    mutable std::vector<pw::link> _links;
};
