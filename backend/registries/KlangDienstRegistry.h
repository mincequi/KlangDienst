#pragma once

#include <rpp/observables/connectable_observable.hpp>
#include <rpp/observables/dynamic_observable.hpp>
#include <rpp/operators/filter.hpp>
#include <rpp/operators/map.hpp>
#include <rpp/operators/publish.hpp>
#include <rpp/sources/just.hpp>
#include <rpp/subjects/publish_subject.hpp>
#include <rpp/subjects/replay_subject.hpp>

#include "MediaClass.h"
#include "MediaRole.h"
#include "MetadataName.h"
#include "MetadataRegistry.h"
#include "NodeRegistry.h"

using namespace rpp;
using namespace rpp::subjects;
using namespace rpp::operators;

class KlangDienstRegistry {
public:
    KlangDienstRegistry(const MetadataRegistry<MetadataName::Default>& metadataRegistry,
                        const NodeRegistry<MediaClass::AudioSink>& audioSinkRegistry,
                        const NodeRegistry<MediaRole::DSP>& dspRegistry) :
        _metadataRegistry(metadataRegistry),
        _audioSinkRegistry(audioSinkRegistry),
        _dspRegistry(dspRegistry),

        _defaultSinkInfoObservable(_metadataRegistry.metadataAdded()
            /*| filter([this](const auto& metadata) {
                return _defaultSinkName.empty();
            })*/ | map([&](const auto& metadata) {
                auto props = metadata.second;
                _defaultSinkName = json::parse(props.at("default.audio.sink").value).value("name", "");
                //if (sinkName.empty()) sinkName = json::parse(props.at("default.audio.sink").value).value("name", "");
                return audioSinkRegistry.nodeInfo(_defaultSinkName);
            }) | filter([this](const auto& sinkInfo) {
                return sinkInfo.has_value();
            }) | map([this](const auto& sinkInfo) {
                return *sinkInfo;
            })),

        _klangDienstSinkInfoObservable(_audioSinkRegistry.nodeAdded()
            | filter([](const auto& media) {
                return (media.props().contains("node.name") &&
                        media.props().at("node.name") == "KlangDienstSink");
            }) | map([](const auto& media) {
                  return media.info();
            })),

        _klangDienstDspInfoObservable(_dspRegistry.nodeAdded()
            | map([](const auto& media) {
                  return media.info();
            })) {
    }

    inline const rpp::dynamic_observable<pw::node_info> defaultSinkInfo() const {
        return _defaultSinkInfoObservable;
    }

    inline const rpp::dynamic_observable<pw::node_info> klangDienstSinkInfo() const {
        return _klangDienstSinkInfoObservable;
    }

    inline const rpp::dynamic_observable<pw::node_info> klangDienstDspInfo() const {
        return _klangDienstDspInfoObservable;
    }

private:
    const MetadataRegistry<MetadataName::Default>& _metadataRegistry;
    const NodeRegistry<MediaClass::AudioSink>& _audioSinkRegistry;
    const NodeRegistry<MediaRole::DSP>& _dspRegistry;

    dynamic_observable<pw::node_info> _defaultSinkInfoObservable;
    dynamic_observable<pw::node_info> _klangDienstSinkInfoObservable;
    dynamic_observable<pw::node_info> _klangDienstDspInfoObservable;

    std::string _defaultSinkName;
};
