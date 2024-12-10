#include "WebServer.h"

#include <cmrc/cmrc.hpp>
#include <drogon/drogon.h>

using namespace drogon;

CMRC_DECLARE(webapp);

class WebServer::Impl {
public:
    Impl() {
        _fs = std::make_shared<cmrc::embedded_filesystem>(cmrc::webapp::get_filesystem());
    }

    void start() {
        app().setLogPath("./")
            .setLogLevel(trantor::Logger::kWarn)
            .addListener("0.0.0.0", 8033)
            .registerHandler("/{file_name}", [this](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
                onRequest(req, std::move(callback));
            },{Get})
            .registerHandler("/assets/{file_name}", [this](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
                onRequest(req, std::move(callback));
            },{Get})
            .registerHandler("/assets/fonts/{file_name}", [this](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
                onRequest(req, std::move(callback));
            },{Get})
            .registerHandler("/assets/packages/{file_name}", [this](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
                onRequest(req, std::move(callback));
            },{Get})
            .registerHandler("/assets/shaders/{file_name}", [this](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
                onRequest(req, std::move(callback));
            },{Get})
            //.setThreadNum(3)
            //.enableRunAsDaemon()
            .run();
    }

    void onRequest(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
        if (req->path() == "/") {
            req->setPath("/index.html");
        }
        auto file_name = req->path();
        HttpResponsePtr response = HttpResponse::newHttpResponse();
        if (!_fs->exists(file_name)) {
            response = HttpResponse::newNotFoundResponse();
        } else {
            auto file = _fs->open(file_name);
            response->setBody(std::string(file.begin(), file.end()));
        }
        callback(response);
    }

    std::shared_ptr<cmrc::embedded_filesystem> _fs;
};

WebServer::WebServer() {
    _impl = std::make_shared<WebServer::Impl>();
}

void WebServer::start() {
    _impl->start();
}


