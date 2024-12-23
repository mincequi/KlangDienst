#include <rohrkabel/context.hpp>
#include <rohrkabel/loop.hpp>
#include <rohrkabel/core/core.hpp>
#include <rohrkabel/link/link.hpp>
#include <rohrkabel/metadata/metadata.hpp>
#include <rohrkabel/node/node.hpp>
#include <rohrkabel/port/port.hpp>
#include <rohrkabel/registry/registry.hpp>
#include <rpp/observables/dynamic_observable.hpp>
#include <spdlog/spdlog.h>

#include "backend/KlangDienstDsp.h"
#include "backend/KlangDienstSink.h"
#include "backend/MediaClass.h"
#include "backend/MediaRole.h"
#include "backend/registries/KlangDienstRegistry.h"
#include "backend/registries/MetadataRegistry.h"
#include "backend/registries/NodeRegistry.h"
#include "backend/registries/PortRegistry.h"
#include "backend/usecases/BuildGraphUseCase.h"
#include "backend/usecases/LinkNodesUseCase.h"
#include "common/Persistence.h"
#include "frontend/WebServer.h"
#include "frontend/WebSocketHandler.h"

bool doRun = true;

static void do_quit(void* userdata, int signal_number) {
    pw_main_loop_quit((pw_main_loop*)userdata);
    doRun = false;
}

namespace pw = pipewire;
using namespace spdlog;

int main(int argc, char *argv[]) {

    auto main_loop = pw::main_loop::create();
    auto context   = pw::context::create(main_loop);
    auto core      = pw::core::create(context);
    auto reg       = pw::registry::create(core);

    pw_loop_add_signal(main_loop->loop(), SIGINT, do_quit, main_loop->get());
    pw_loop_add_signal(main_loop->loop(), SIGTERM, do_quit, main_loop->get());

    Persistence persistence;
    KlangDienstDsp dsp(main_loop, persistence);
    KlangDienstSink sink(core);

    // Find ports to nodes
    Registry<pw::port> portRegistry(*reg);
    PortRegistry portRegistryProxy(portRegistry);

    // Find relevant nodes
    Registry<pw::node> nodeRegistry(*reg);
    NodeRegistry<MediaClass::AudioSink> audioSinkRegistry(nodeRegistry);
    audioSinkRegistry.nodeAdded().subscribe([&](const auto& sink) {
        info("audioSinkRegistry> added: {}", name(sink));
    });
    NodeRegistry<MediaRole::DSP> dspRegistry(nodeRegistry, "KlangDienstDsp");

    // Find default sink
    Registry<pw::metadata> metadataRegistry(*reg);
    MetadataRegistry<MetadataName::Default> defaultMetadataRegistry(metadataRegistry);
    defaultMetadataRegistry.metadataAdded().subscribe([](const auto& metadata) {
        info("defaultMetadataRegistry> added: {}", metadata.first);
        for (const auto& p : metadata.second) {
            info("  {}: {}", p.first, p.second.value);
        }
    });

    KlangDienstRegistry klangDienstRegistry(defaultMetadataRegistry, audioSinkRegistry, dspRegistry);
    LinkNodesUseCase linkNodesUseCase(core, portRegistryProxy);
    BuildGraphUseCase buildGraphUseCase(defaultMetadataRegistry, klangDienstRegistry, linkNodesUseCase);

    auto webSocketHandler = std::make_shared<WebSocketHandler>(dsp);
    WebServer webServer(webSocketHandler);
    auto webServerThread = std::thread([&]() {
        webServer.start();
    });

    pw_loop_enter(main_loop->loop());
    while (doRun) {
        pw_loop_iterate(main_loop->loop(), -1);
    }

    return 0;
}
