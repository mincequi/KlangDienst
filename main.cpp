#include <rohrkabel/context.hpp>
#include <rohrkabel/loop.hpp>
#include <rohrkabel/core/core.hpp>
#include <rohrkabel/metadata/metadata.hpp>
#include <rohrkabel/node/node.hpp>
#include <rohrkabel/registry/registry.hpp>

#include <spdlog/spdlog.h>

#include "AudioSink.h"
#include "EqualizerNode.h"
#include "Link.h"
#include "Metadata.h"
#include "Registry.h"
#include "Repository.h"
#include "StreamOutputAudio.h"

static void do_quit(void* userdata, int signal_number) {
    pw_main_loop_quit((pw_main_loop*)userdata);
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

    //EqualizerNode node(main_loop);

    Registry<AudioSink> audioSinkRegistry(core);
    audioSinkRegistry.interfaceAdded().subscribe([](const AudioSink& sink) {
        info("audioSinkRegistry> added: {}", sink.id());
        for (const auto& p : sink.props()) {
            info("  {}: {}", p.first, p.second);
        }
    });

    Registry<StreamOutputAudio> streamOutputAudioRegistry(core);
    streamOutputAudioRegistry.interfaceAdded().subscribe([](const StreamOutputAudio& stream) {
        info("streamOutputAudioRegistry> added: {}", stream.id());
        for (const auto& p : stream.props()) {
            info("  {}: {}", p.first, p.second);
        }
    });
    streamOutputAudioRegistry.interfaceRemoved().subscribe([](const StreamOutputAudio& stream) {
        info("streamOutputAudioRegistry> removed: {}", stream.id());
    });

    Registry<Link> linkRegistry(core);
    linkRegistry.interfaceAdded().subscribe([](const Link& link) {
        info("linkRegistry> added: {}", link.id());
        for (const auto& p : link.props()) {
            info("  {}: {}", p.first, p.second);
        }
    });
    linkRegistry.interfaceRemoved().subscribe([](const Link& link) {
        info("linkRegistry> removed: {}", link.id());
    });

    /*Repository<pw::node> nodeRepository(*reg);
    nodeRepository.proxyAdded().subscribe([](const pw::node& node) {
        info("nodeRepository> added: {}", node.id());
        for (const auto& p : node.props()) {
            info("  {}: {}", p.first, p.second);
        }
    });*/

    main_loop->run();
    return 0;
}
