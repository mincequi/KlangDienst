#pragma once

#include <nlohmann/json.hpp>

#include <rohrkabel/metadata/metadata.hpp>

#include <rpp/observables/dynamic_observable.hpp>
#include <rpp/subjects/publish_subject.hpp>

#include <spdlog/spdlog.h>

#include "MetadataName.h"
#include "Registry.h"

using namespace spdlog;
using json = nlohmann::json;

template <MetadataName N>
class MetadataRegistry {
public:
    MetadataRegistry(Registry<pw::metadata>& metadataRepository) {
        metadataRepository.proxyAdded().subscribe([&](const pw::metadata& node) {
            if (!isName(node)) return;

            _nodes.emplace(node.id(), metadataRepository.bind(node.id()));
            _metadataAddedSubject.get_observer().on_next(node);
        });

        metadataRepository.proxyRemoved().subscribe([this](const pw::metadata& node) {
            if (!_nodes.contains(node.id())) return;

            _nodes.erase(node.id());
            _metadataRemovedSubject.get_observer().on_next(node);
        });
    }

    inline const std::map<std::uint32_t, pw::metadata>& metadata() const {
        return _nodes;
    }

    inline const rpp::dynamic_observable<pw::metadata> metadataAdded() const {
        return _metadataAddedSubject.get_observable();
    }

    inline const rpp::dynamic_observable<pw::metadata> metadataRemoved() const {
        return _metadataRemovedSubject.get_observable();
    }

    inline void setDefaultSink(const std::string& sinkName) const {
        for (auto& [id, metadata] : _nodes) {
            if (metadata.properties().contains("default.audio.sink")) {
                json sinkJson;
                sinkJson["name"] = sinkName;
                metadata.set_property(0, //id,
                                      "default.configured.audio.sink",
                                      metadata.properties().at("default.audio.sink").type,
                                      sinkJson.dump());
                info("MetadataRepository> setDefaultSink: {}", sinkName);
                return;
            }
        }
        warn("MetadataRepository> no default audio sink found");
    }

private:
    static bool isName(const pw::metadata& node) {
        auto type = "metadata.name";

        if (!node.props().contains(type)) return false;
        if (node.props().at(type) != toString(N)) return false;

        return true;
    }

    mutable std::map<std::uint32_t, pw::metadata> _nodes;
    rpp::subjects::publish_subject<pw::metadata> _metadataAddedSubject;
    rpp::subjects::publish_subject<pw::metadata> _metadataRemovedSubject;
};
