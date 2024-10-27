#pragma once

#include <rpp/subjects/publish_subject.hpp>

#include <rohrkabel/node/node.hpp>
#include <rohrkabel/proxy/proxy.hpp>
#include <rohrkabel/registry/events.hpp>
#include <rohrkabel/registry/registry.hpp>

namespace ph = std::placeholders;
namespace pw = pipewire;

template <typename T>
concept IsProxy = requires {
    { std::is_base_of_v<pw::proxy, T> };
};

template <IsProxy T>
class Repository {
public:
    Repository(pw::registry& registry) :
        //_core(core),
        //_registry(pw::registry::create(core)),
        _registry(registry),
        _listener(_registry.listen()) {
        _listener.on<pipewire::registry_event::global>(std::bind(&Repository::onGlobal, this, ph::_1));
        _listener.on<pipewire::registry_event::global_removed>(std::bind(&Repository::onGlobalRemoved, this, ph::_1));
    }

    inline const rpp::dynamic_observable<T> proxyAdded() {
        return _proxyAddedSubject.get_observable();
    }

    inline const rpp::dynamic_observable<T> proxyRemoved() {
        return _proxyRemovedSubject.get_observable();
    }

private:
    void onGlobal(const pw::global& global) {
        if (global.type != T::type) return;

        auto p = *_registry.bind<T>(global.id).get();
        //_proxyAddedSubject.get_observer().on_next(p);
        //_proxies.emplace(global.id, std::move(p));
        //_proxies.emplace(global.id, *_registry.bind<T>(global.id).get());
    }

    void onGlobalRemoved(std::uint32_t id) {
        if (!_proxies.contains(id)) return;

        _proxyRemovedSubject.get_observer().on_next(_proxies.at(id));
        _proxies.erase(id);
    }

    std::shared_ptr<pw::core> _core;
    pw::registry& _registry;
    pw::registry_listener _listener;
    std::map<std::uint32_t, T> _proxies;

    rpp::subjects::publish_subject<T> _proxyAddedSubject;
    rpp::subjects::publish_subject<T> _proxyRemovedSubject;
};
