#pragma once

#include <rohrkabel/metadata/metadata.hpp>

#include <rpp/observables/dynamic_observable.hpp>
#include <rpp/subjects/publish_subject.hpp>

#include <spdlog/spdlog.h>

#include "MetadataName.h"
#include "Repository.h"

using namespace spdlog;

template <MetadataName N>
class MetadataRepository {
public:
    MetadataRepository(Repository<pw::metadata>& metadataRepository) {
        metadataRepository.proxyAdded().subscribe([&](const pw::metadata& node) {
            if (!isName(node)) return;

            _metadata.emplace(node.id(), metadataRepository.bind(node.id()));
            _metadataAddedSubject.get_observer().on_next(node);
        });

        metadataRepository.proxyRemoved().subscribe([this](const pw::metadata& node) {
            if (!_metadata.contains(node.id())) return;

            _metadata.erase(node.id());
            _metadataRemovedSubject.get_observer().on_next(node);
        });
    }

    inline const std::map<std::uint32_t, pw::metadata>& metadata() const {
        return _metadata;
    }

    inline const rpp::dynamic_observable<pw::metadata> metadataAdded() const {
        return _metadataAddedSubject.get_observable();
    }

    inline const rpp::dynamic_observable<pw::metadata> metadataRemoved() const {
        return _metadataRemovedSubject.get_observable();
    }

    inline void setDefaultSink(const std::string& sink) const {
        for (auto& [id, metadata] : _metadata) {
            if (metadata.properties().contains("default.audio.sink")) {
                metadata.set_property(id,
                                      "default.audio.sink",
                                      metadata.properties().at("default.audio.sink").type,
                                      sink);
                info("MetadataRepository> set default audio sink: {}", sink);
                return;
            }
        }
        warn("MetadataRepository> no default audio sink found");
    }

private:
    static bool isName(const pw::metadata& node) {
        if (!node.props().contains("metadata.name")) return false;
        if (node.props().at("metadata.name") != toString(N)) return false;

        return true;
    }

    mutable std::map<std::uint32_t, pw::metadata> _metadata;
    rpp::subjects::publish_subject<pw::metadata> _metadataAddedSubject;
    rpp::subjects::publish_subject<pw::metadata> _metadataRemovedSubject;
};
