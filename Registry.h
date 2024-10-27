#pragma once

#include <set>

#include <rpp/rpp.hpp>
#include <rpp/subjects/publish_subject.hpp>

#include <rohrkabel/registry/events.hpp>
#include <rohrkabel/registry/registry.hpp>

#include "Interface.h"
#include "InterfaceType.h"

namespace pw = pipewire;

template <typename T>
concept IsInterface = requires {
    { std::is_base_of_v<Interface, T> };
    { T::type() } -> std::same_as<InterfaceType>;
};

template <IsInterface T>
class Registry {
public:
    Registry(std::shared_ptr<pw::core>);

    inline const rpp::dynamic_observable<T> interfaceAdded() {
        return _interfaceAddedSubject.get_observable();
    }

    inline const rpp::dynamic_observable<T> interfaceRemoved() {
        return _interfaceRemovedSubject.get_observable();
    }

private:
    void onGlobal(const pw::global& global);
    void onGlobalRemoved(std::uint32_t id);

    std::shared_ptr<pw::core> _core;
    std::optional<pw::registry> _registry;
    pw::registry_listener _listener;

    std::map<std::uint32_t,T> _interfaces;
    rpp::subjects::publish_subject<T> _interfaceAddedSubject;
    rpp::subjects::publish_subject<T> _interfaceRemovedSubject;
};
