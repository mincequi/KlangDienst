#pragma once

#include <magic_enum.hpp>

#include <rohrkabel/node/node.hpp>

#include <rpp/observables/dynamic_observable.hpp>
#include <rpp/subjects/publish_subject.hpp>

#include "Registry.h"

template <auto C>
class NodeRegistry {
public:
    NodeRegistry(Registry<pw::node>& nodeRepository, const std::string& nameFilter = "") :
        _nameFilter(nameFilter) {
        nodeRepository.proxyAdded().subscribe([&](const pw::node& node) {
            if (!isRelevant(node)) return;

            _nodes.emplace(node.id(), nodeRepository.bind(node.id()));
            _mediaAddedSubject.get_observer().on_next(node);
        });

        nodeRepository.proxyRemoved().subscribe([this](const pw::node& node) {
            if (!_nodes.contains(node.id())) return;

            _nodes.erase(node.id());
            _mediaRemovedSubject.get_observer().on_next(node);
        });
    }

    inline const std::map<std::uint32_t, pw::node>& media() const {
        return _nodes;
    }

    inline const rpp::dynamic_observable<pw::node> mediaAdded() const {
        return _mediaAddedSubject.get_observable();
    }

    inline const rpp::dynamic_observable<pw::node> mediaRemoved() const {
        return _mediaRemovedSubject.get_observable();
    }

private:
    bool isRelevant(const pw::node& node) {
        const auto type = typeName();
        if (!node.props().contains(type)) return false;
        if (node.props().at(type) != toString(C)) return false;

        if (!_nameFilter.empty() && !node.props().contains("node.name")) return false;
        if (!_nameFilter.empty() && node.props().at("node.name") != _nameFilter) return false;

        return true;
    }

    static constexpr std::string typeName() {
        // Convert Camelcase to lower case and insert '.' before uppercase letters
        std::string typeName(magic_enum::enum_type_name<decltype(C)>());
        for (auto i = 1; i < typeName.size(); ++i) {
            if (std::isupper(typeName[i])) {
                typeName.insert(i++, 1, '.');
            }
        }
        std::transform(typeName.begin(), typeName.end(), typeName.begin(), ::tolower);
        return typeName;
    }

    static std::string toString(auto media) {
        auto m = std::string(magic_enum::enum_name(media));
        for (auto i = 1; i < m.size()-1; ++i) {
            if (std::isupper(m[i]) && !std::isupper(m[i+1])) {
                m.insert(i++, 1, '/');
            }
        }
        return m;
    }

    std::string _nameFilter;
    std::map<std::uint32_t, pw::node> _nodes;
    rpp::subjects::publish_subject<pw::node> _mediaAddedSubject;
    rpp::subjects::publish_subject<pw::node> _mediaRemovedSubject;
};
