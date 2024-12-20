#pragma once

#include <nlohmann/json.hpp>

#include <rohrkabel/metadata/metadata.hpp>

#include <rpp/observables/dynamic_observable.hpp>
#include <rpp/subjects/publish_subject.hpp>
#include <rpp/subjects/replay_subject.hpp>

#include <spdlog/spdlog.h>

#include "MetadataName.h"
#include "Registry.h"

using namespace spdlog;
using json = nlohmann::json;

template <MetadataName N>
class MetadataRegistry {
public:
    using MetadataInfo = std::pair<std::uint32_t,std::map<std::string, pw::metadata_property>>;

    MetadataRegistry(Registry<pw::metadata>& metadataRepository) {
        metadataRepository.proxyAdded().subscribe([&](const pw::metadata& node) {
            if (!isName(node)) return;

            _metadata.emplace(node.id(), metadataRepository.bind(node.id()));
            _metadataAddedSubject.get_observer().on_next({node.id(), node.properties()});
        });

        metadataRepository.proxyRemoved().subscribe([this](const pw::metadata& node) {
            if (!_metadata.contains(node.id())) return;

            _metadata.erase(node.id());
            _metadataRemovedSubject.get_observer().on_next({node.id(), node.properties()});
        });
    }

    inline const std::map<std::uint32_t, pw::metadata>& metadata() const {
        return _metadata;
    }

    inline const rpp::dynamic_observable<MetadataInfo> metadataAdded() const {
        return _metadataAddedSubject.get_observable();
    }

    inline const rpp::dynamic_observable<MetadataInfo> metadataRemoved() const {
        return _metadataRemovedSubject.get_observable();
    }

    inline void setDefaultSink(const std::string& sinkName) const {
        for (auto& [id, metadata] : _metadata) {
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

    mutable std::map<std::uint32_t, pw::metadata> _metadata;
    rpp::subjects::publish_subject<MetadataInfo> _metadataAddedSubject;
    rpp::subjects::publish_subject<MetadataInfo> _metadataRemovedSubject;
};
