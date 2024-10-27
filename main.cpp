#include <rpp/observables/dynamic_observable.hpp>

#include <rohrkabel/context.hpp>
#include <rohrkabel/loop.hpp>
#include <rohrkabel/core/core.hpp>
#include <rohrkabel/link/link.hpp>
#include <rohrkabel/metadata/metadata.hpp>
#include <rohrkabel/node/node.hpp>
#include <rohrkabel/registry/registry.hpp>

#include <spdlog/spdlog.h>

#include "BuildGraph.h"
#include "EqualizerNode.h"
#include "KlangDienstSink.h"
#include "MediaRepository.h"
#include "MetadataRepository.h"

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

    EqualizerNode node(main_loop);
    KlangDienstSink sink(*core);

    Repository<pw::node> nodeRepository(*reg);

    MediaRepository<MediaClass::AudioSink> audioSinkRepository(nodeRepository);
    /*audioSinkRepository.mediaAdded().subscribe([](const auto& sink) {
        info("audioSinkRepository> added: {}", sink.id());
        for (const auto& p : sink.props()) {
            info("  {}: {}", p.first, p.second);
        }
    });*/
    MediaRepository<MediaClass::StreamOutputAudio> streamOutputAudioRepository(nodeRepository);
    streamOutputAudioRepository.mediaAdded().subscribe([](const auto& stream) {
        info("streamOutputAudioRepository> added: {}", stream.id());
        for (const auto& p : stream.props()) {
            //info("  {}: {}", p.first, p.second);
        }
    });

    Repository<pw::metadata> metadataRepository(*reg);
    MetadataRepository<MetadataName::Default> defaultMetadataRepository(metadataRepository);
    defaultMetadataRepository.metadataAdded().subscribe([](const auto& metadata) {
        info("defaultMetadataRepository> added: {}", metadata.id());
        for (const auto& p : metadata.properties()) {
            info("  {}: {}", p.first, p.second.value);
        }
        for (const auto& p : metadata.props()) {
            info("  {}: {}", p.first, p.second);
        }
    });

    BuildGraph buildGraph(defaultMetadataRepository, audioSinkRepository);

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
