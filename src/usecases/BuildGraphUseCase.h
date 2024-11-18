#pragma once

#include <nlohmann/json.hpp>
#include <rpp/observables/observable.hpp>
#include <rpp/operators/combine_latest.hpp>
#include <rpp/operators/filter.hpp>
#include <rpp/operators/map.hpp>
#include <rpp/sources/just.hpp>
#include <spdlog/spdlog.h>

#include "Util.h"
#include "registries/KlangDienstRegistry.h"
#include "registries/MetadataRegistry.h"
#include "usecases/LinkNodesUseCase.h"

using json = nlohmann::json;
using namespace rpp::operators;
using namespace spdlog;

class BuildGraphUseCase {
public:
    BuildGraphUseCase(const MetadataRegistry<MetadataName::Default>& metadataRepository,
                      const KlangDienstRegistry& klangDienstRegistry,
                      const LinkNodesUseCase& linkNodesUseCase) :
        _metadataRepository(metadataRepository),
        _klangDienstRegistry(klangDienstRegistry),
        _linkNodesUseCase(linkNodesUseCase) {

        wirePipe();
    }

    ~BuildGraphUseCase() {
        if (_defaultSinkName.empty()) return;
        _metadataRepository.setDefaultSink(_defaultSinkName);
    }

private:
    void wirePipe() {
        // 1. Set KlangDienstSink as default sink (and store the old one)
        _klangDienstRegistry.defaultSinkInfo()
            | map([](const auto& defaultSink) { return name(defaultSink); })
            | combine_latest([this](const auto& defaultSinkName, const auto& klangDienstSink) {
                info("BuildGraphUseCase> change default sink: {} -> {}", defaultSinkName, klangDienstSink);
                _metadataRepository.setDefaultSink(klangDienstSink);
                _defaultSinkName = defaultSinkName;
                return defaultSinkName;
            }, _klangDienstRegistry.klangDienstSinkInfo()
            | map([](const auto& klangDienstSink) { return name(klangDienstSink); }))
            | subscribe([this](const auto& _) {});

        // 2. Link KlangDienstSink to KlangeDienstDsp
        _klangDienstRegistry.klangDienstSinkInfo()
            | combine_latest([this](const auto& klangDienstSink, const auto& klangDienstDsp) {
                info("BuildGraphUseCase> link: {} -> {}", name(klangDienstSink), name(klangDienstDsp));
                _linkNodesUseCase(klangDienstSink.id, klangDienstDsp.id);
                return true;
            }, _klangDienstRegistry.klangDienstDspInfo())
            | subscribe([](const auto& _) {});

        // 3. Link KlangeDienstDsp to old default sink
        _klangDienstRegistry.klangDienstDspInfo()
            | combine_latest([this](const auto& klangDienstDsp, const auto& defaultSink) {
                  info("BuildGraphUseCase> link: {} -> {}", name(klangDienstDsp), name(defaultSink));
                  _linkNodesUseCase(klangDienstDsp.id, defaultSink.id);
                  return true;
              }, _klangDienstRegistry.defaultSinkInfo())
            | subscribe([](const auto& _) {});
    }

    std::string _defaultSinkName;
    const MetadataRegistry<MetadataName::Default>& _metadataRepository;
    const KlangDienstRegistry& _klangDienstRegistry;
    const LinkNodesUseCase& _linkNodesUseCase;
};
