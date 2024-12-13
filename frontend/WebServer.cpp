#include "WebServer.h"

#include <cmrc/cmrc.hpp>
#include <drogon/drogon.h>

#include "WebSocketHandler.h"

using namespace drogon;

CMRC_DECLARE(webapp);

WebServer::WebServer(std::shared_ptr<WebSocketHandler> webSocketHandler) : _webSocketHandler(webSocketHandler) {
    _fs = std::make_unique<cmrc::embedded_filesystem>(cmrc::webapp::get_filesystem());
}

WebServer::~WebServer() {
}

void WebServer::start() {
    app().setLogPath("./")
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8033)
        .setDefaultHandler([this](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
            if (req->path() == "/") {
                req->setPath("/index.html");
            }
            auto fileName = req->path();
            HttpResponsePtr response = HttpResponse::newHttpResponse();
            if (!_fs->exists(fileName)) {
                response = HttpResponse::newNotFoundResponse();
            } else {
                auto file = _fs->open(fileName);
                response->setBody(std::string(file.begin(), file.end()));
            }
            callback(response);
        })
        .registerController(_webSocketHandler)
        //.setThreadNum(3)
        //.enableRunAsDaemon()
        .run();
}


