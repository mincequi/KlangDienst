#include <rpp/observables/dynamic_observable.hpp>

#include <rohrkabel/context.hpp>
#include <rohrkabel/loop.hpp>
#include <rohrkabel/core/core.hpp>
#include <rohrkabel/link/link.hpp>
#include <rohrkabel/metadata/metadata.hpp>
#include <rohrkabel/node/node.hpp>
#include <rohrkabel/port/port.hpp>
#include <rohrkabel/registry/registry.hpp>

#include <spdlog/spdlog.h>

#include "Graph.h"
#include "KlangDienstDsp.h"
#include "KlangDienstSink.h"
#include "MediaClass.h"
#include "MediaRole.h"
#include "NodeRegistry.h"
#include "MetadataRegistry.h"
#include "PortRegistry.h"
#include "use_cases/LinkNodesUseCase.h"

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

    KlangDienstDsp dsp(main_loop);
    KlangDienstSink sink(core);

    // Find ports to nodes
    Registry<pw::port> portRegistry(*reg);
    PortRegistry portRegistryProxy(portRegistry);

    // Find relevant nodes
    Registry<pw::node> nodeRegistry(*reg);
    NodeRegistry<MediaClass::AudioSink> audioSinkRegistry(nodeRegistry);
    audioSinkRegistry.mediaAdded().subscribe([&](const auto& sink) {
        info("audioSinkRegistry> added: {}", sink.id());
        if (sink.props().contains("node.name")) {
            info("  node.name: {}", sink.props().at("node.name"));
        }
        //auto ports = portRegistryProxy.ports(sink);
        //for (const auto& port : ports) {
        //    info("  port: {}", port.id);
        //    for (const auto& p : port.props) {
        //        info("    {}: {}", p.first, p.second);
        //    }
        //}
    });
    NodeRegistry<MediaRole::DSP> dspRegistry(nodeRegistry, "KlangDienstFilter");
    dspRegistry.mediaAdded().subscribe([&](const auto& dsp) {
        info("dspRegistry> added: {}", dsp.id());
        for (const auto& p : dsp.props()) {
            info("  {}: {}", p.first, p.second);
        }
        //auto ports = portRegistryProxy.ports(dsp);
        //for (const auto& port : ports) {
        //    info("  port: {}", port.id);
        //    for (const auto& p : port.props) {
        //        info("    {}: {}", p.first, p.second);
        //    }
        //}
    });

    // Find default sink
    Registry<pw::metadata> metadataRegistry(*reg);
    MetadataRegistry<MetadataName::Default> defaultMetadataRegistry(metadataRegistry);
    defaultMetadataRegistry.metadataAdded().subscribe([](const auto& metadata) {
        info("defaultMetadataRegistry> added: {}", metadata.id());
        for (const auto& p : metadata.properties()) {
            info("  {}: {}", p.first, p.second.value);
        }
    });

    //PortRepository portRegistryProxy(portRegistry);
    //portRegistryProxy.portAdded().subscribe([](const auto& port) {
    //    info("portRegistryProxy> added: {}", port.id());
    //    for (const auto& p : port.props()) {
    //        info("  {}: {}", p.first, p.second);
    //    }
    //});
    //audioSinkRegistry.mediaAdded().subscribe([&](const auto& sink) {
    //    portRegistryProxy.add(sink);
    //});
    //dspRegistry.mediaAdded().subscribe([&](const auto& dsp) {
    //    portRegistryProxy.add(dsp);
    //});

    Graph buildGraph(defaultMetadataRegistry, audioSinkRegistry);

    pw_loop_enter(main_loop->loop());
    while (doRun) {
        pw_loop_iterate(main_loop->loop(), -1);
    }

    //while (true) {
    //    core->update();
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //}
    //main_loop->run();
    return 0;
}
