#include "WebServer.h"

#include <drogon/drogon.h>

using namespace drogon;

WebServer::WebServer(KlangDienstDsp& dsp) :
    _dsp(dsp) {
}

void WebServer::start() {
    _worker = std::thread([]() {
        app().setLogPath("./")
        .setLogLevel(trantor::Logger::kWarn)
            .addListener("0.0.0.0", 8033)
            //.setThreadNum(3)
            //.enableRunAsDaemon()
            .run();
    });
}
