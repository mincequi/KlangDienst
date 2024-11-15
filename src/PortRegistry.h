#pragma once

#include <magic_enum.hpp>
#include <rohrkabel/node/node.hpp>
#include <rohrkabel/port/port.hpp>
#include <rpp/observables/dynamic_observable.hpp>
#include <rpp/subjects/publish_subject.hpp>

#include "AudioChannel.h"
#include "Registry.h"

class PortRegistry {
public:
    PortRegistry(Registry<pw::port>& portRegistry) {
        portRegistry.proxyAdded().subscribe([&](const pw::port& port) {
            if (!isRelevant(port)) return;

            _ports.emplace(port.id(), portRegistry.bind(port.id()));
            _portAddedSubject.get_observer().on_next(port);
        });

        portRegistry.proxyRemoved().subscribe([this](const pw::port& node) {
            if (!_ports.contains(node.id())) return;

            _ports.erase(node.id());
            _portRemovedSubject.get_observer().on_next(node);
        });
    }

    // Get port for a specific node, direction and channel
    std::optional<pw::port_info> port(const pw::node& node,
                                      pw::port_direction direction,
                                      AudioChannel channel) const {
        for (const auto& [id, port] : _ports) {
            if (port.props().contains("node.id") &&
                std::stoul(port.props().at("node.id")) == node.id() &&
                port.info().direction == direction &&
                port.props().contains("audio.channel") &&
                port.props().at("audio.channel") == magic_enum::enum_name(channel)) {
                return port.info();
            }
        }
        return std::nullopt;
    }

    inline const std::map<std::uint32_t, pw::port>& ports() const {
        return _ports;
    }

    inline const rpp::dynamic_observable<pw::port> portAdded() const {
        return _portAddedSubject.get_observable();
    }

    inline const rpp::dynamic_observable<pw::port> portRemoved() const {
        return _portRemovedSubject.get_observable();
    }

private:
    bool isRelevant(const pw::port& port) {
        return port.props().contains("node.id");
    }

    std::map<std::uint32_t, pw::port> _ports;
    rpp::subjects::publish_subject<pw::port> _portAddedSubject;
    rpp::subjects::publish_subject<pw::port> _portRemovedSubject;
};
