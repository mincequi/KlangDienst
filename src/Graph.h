#pragma once

#include <nlohmann/json.hpp>

#include <rpp/observables/observable.hpp>
#include <rpp/operators/combine_latest.hpp>
#include <rpp/operators/filter.hpp>
#include <rpp/operators/map.hpp>
#include <rpp/sources/just.hpp>

#include <spdlog/spdlog.h>

#include "MediaClass.h"
#include "NodeRegistry.h"
#include "MetadataRegistry.h"
#include "use_cases/LinkNodesUseCase.h"

using json = nlohmann::json;
using namespace rpp::operators;
using namespace spdlog;

class Graph {
public:
    Graph(const MetadataRegistry<MetadataName::Default>& metadataRepository,
               const NodeRegistry<MediaClass::AudioSink>& audioSinkRepository) :
        _metadataRepository(metadataRepository),
        _audioSinkRepository(audioSinkRepository),

        _defaultSinkNameObservable(_metadataRepository.metadataAdded() | map([](const auto& metadata) {
            return metadata.properties();
        }) | filter([this](const auto& props) {
            return (props.contains("default.configured.audio.sink") ||
                    props.contains("default.audio.sink")) &&
                   _defaultSink.empty();
        }) | map([this](const auto& props) {
            std::string sinkName = json::parse(props.at("default.configured.audio.sink").value).value("name", "");
            if (!sinkName.empty()) return sinkName;
            return json::parse(props.at("default.audio.sink").value).value("name", "");
        }) | filter([](const auto& name) {
            return !name.empty();
        })),

        _klangDienstSinkObservable(_audioSinkRepository.mediaAdded() | filter([](const auto& media) {
            return (media.props().contains("node.name") &&
                    media.props().at("node.name") == "KlangDienstSink");
        }) | map([](const auto& media) {
            return media.props().at("node.name");
        })) {

        wirePipe();
    }

    ~Graph() {
        if (_defaultSink.empty()) return;
        _metadataRepository.setDefaultSink(_defaultSink);
    }

private:
    void wirePipe() {
        // 1. Set KlangeDienstSink as default sink (and store the old one)
        _defaultSinkNameObservable
            | combine_latest([this](const auto& defaultSink, const auto& klangDienstSink) {
                  info("BuildGraph> new default sink: {}", klangDienstSink);
                  _metadataRepository.setDefaultSink(klangDienstSink);
                  return defaultSink;
              }, _klangDienstSinkObservable)
            | subscribe([this](const auto& defaultSink) {
                  info("BuildGraph> old default sink: {}", defaultSink);
                  _defaultSink = defaultSink;
              });

        // 2. Link KlangDienstSink to KlangeDienstDsp
        _klangDienstSinkObservable
            | subscribe([this](const auto& klangDienstSink) {
                  info("BuildGraph> KlangDienstSink: {}", klangDienstSink);
              });

        // 3. Link KlangeDienstDsp to old default sink
    }

    std::string _defaultSink;
    const MetadataRegistry<MetadataName::Default>& _metadataRepository;
    const NodeRegistry<MediaClass::AudioSink>& _audioSinkRepository;

    rpp::dynamic_observable<std::string> _defaultSinkNameObservable;
    rpp::dynamic_observable<std::string> _klangDienstSinkObservable;

    //LinkNodesUseCase _linkNodesUseCase;
};
