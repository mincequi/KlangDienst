#pragma once

#include <rpp/subjects/publish_subject.hpp>

#include <rohrkabel/proxy/proxy.hpp>
#include <rohrkabel/registry/events.hpp>
#include <rohrkabel/registry/registry.hpp>

#include <spdlog/spdlog.h>

namespace ph = std::placeholders;
namespace pw = pipewire;

using namespace spdlog;

template <typename T>
concept IsProxy = requires {
    { std::is_base_of_v<pw::proxy, T> };
};

template <IsProxy T>
class Repository {
public:
    using U = std::pair<pw::global, T>;

    Repository(pw::registry& registry) :
        _registry(registry),
        _listener(_registry.listen()) {
        _listener.on<pipewire::registry_event::global>(std::bind(&Repository::onGlobal, this, ph::_1));
        _listener.on<pipewire::registry_event::global_removed>(std::bind(&Repository::onGlobalRemoved, this, ph::_1));
    }

    inline const rpp::dynamic_observable<T> proxyAdded() const {
        return _proxyAddedSubject.get_observable();
    }

    inline const rpp::dynamic_observable<T> proxyRemoved() const {
        return _proxyRemovedSubject.get_observable();
    }

    inline T bind(std::uint32_t id) {
        return *_registry.bind<T>(id).get();
    }

private:
    void onGlobal(const pw::global& global) {
        if (global.type != T::type) return;

        info("Repository> onGlobal: {}", global.id);
        info("perm: {:x}", global.permissions);
        for (const auto& p : global.props) {
            info("  {}: {}", p.first, p.second);
        }

        _proxies.emplace(global.id, *_registry.bind<T>(global.id).get());
        //_proxies.emplace(global.id, std::make_pair(global, *_registry.bind<T>(global.id).get()));
        _proxyAddedSubject.get_observer().on_next(_proxies.at(global.id));
    }

    void onGlobalRemoved(std::uint32_t id) {
        if (!_proxies.contains(id)) return;

        _proxyRemovedSubject.get_observer().on_next(_proxies.at(id));
        _proxies.erase(id);
    }

    pw::registry& _registry;
    pw::registry_listener _listener;
    std::map<std::uint32_t, T> _proxies;
    //std::map<std::uint32_t, U> _proxies;

    rpp::subjects::publish_subject<T> _proxyAddedSubject;
    rpp::subjects::publish_subject<T> _proxyRemovedSubject;
};
