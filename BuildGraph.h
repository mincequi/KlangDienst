#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

#include <rpp/observables/observable.hpp>
#include <rpp/operators/combine_latest.hpp>
#include <rpp/operators/filter.hpp>
#include <rpp/operators/map.hpp>
#include <rpp/sources/just.hpp>

#include <spdlog/spdlog.h>

#include "MediaRepository.h"
#include "MetadataRepository.h"

using json = nlohmann::json;
using namespace rpp::operators;
using namespace spdlog;

class BuildGraph {
public:
    BuildGraph(const MetadataRepository<MetadataName::Default>& metadataRepository,
               const MediaRepository<MediaClass::AudioSink>& audioSinkRepository) :
        _metadataRepository(metadataRepository),
        _audioSinkRepository(audioSinkRepository),

        _defaultSinkNameObservable(_metadataRepository.metadataAdded() | map([](const auto& metadata) {
            return metadata.properties();
        }) | filter([this](const auto& props) {
            return props.contains("default.audio.sink") && _defaultSink.empty();
        }) | map([this](const auto& props) {
            std::string defaultSink = props.at("default.audio.sink").value;
            auto json = json::parse(defaultSink);
            return json.value("name", "");
        }) | filter([](const auto& name) {
            return !name.empty();
        })),

        _klangDienstSinkObservable(_audioSinkRepository.mediaAdded() | filter([](const auto& media) {
            return (media.props().contains("node.name") &&
                    media.props().at("node.name") == "KlangDienstSink");
        }) | map([](const auto& media) {
            return media.props().at("node.name");
        })) {

        _defaultSinkNameObservable
         | combine_latest([this](const auto& defaultSink, const auto& klangDienstSink) {
            info("BuildGraph> set new default sink: {}", klangDienstSink);
            _metadataRepository.setDefaultSink(klangDienstSink);
            return defaultSink;
        }, _klangDienstSinkObservable)
            | subscribe([this](const auto& defaultSink) {
            info("BuildGraph> store old default sink: {}", defaultSink);
            _defaultSink = defaultSink;
        });
    }

    ~BuildGraph() {
        _metadataRepository.setDefaultSink(_defaultSink);
    }

    // overload () operator
    void operator()() {

    }

private:
    std::string _defaultSink;
    const MetadataRepository<MetadataName::Default>& _metadataRepository;
    const MediaRepository<MediaClass::AudioSink>& _audioSinkRepository;

    rpp::dynamic_observable<std::string> _defaultSinkNameObservable;
    rpp::dynamic_observable<std::string> _klangDienstSinkObservable;
};
