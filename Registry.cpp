#include "Registry.h"

#include <array>
#include <utility>

#include <magic_enum.hpp>
#include <rpp/observers/observer.hpp>

#include "AudioSink.h"
#include "Link.h"
#include "Metadata.h"
#include "StreamOutputAudio.h"

template <typename T>
concept InheritsNodeBase = std::derived_from<T, NodeBase> && !std::is_same_v<NodeBase, T>;

using namespace std::placeholders;

template <IsInterface T>
Registry<T>::Registry(std::shared_ptr<pw::core> core) :
    _core(core),
    _registry(pw::registry::create(core)),
    _listener(_registry->listen()) {
    _listener.on<pipewire::registry_event::global>(std::bind(&Registry::onGlobal, this, _1));
    _listener.on<pipewire::registry_event::global_removed>(std::bind(&Registry::onGlobalRemoved, this, _1));
}

template <IsInterface T>
void Registry<T>::onGlobal(const pw::global& global) {
    const auto shortName = global.type.substr(global.type.find_last_of(':') + 1);
    if (shortName == magic_enum::enum_name<>(T::type())) {
        auto iface = Interface::fromGlobal(global);
        auto t = T::fromInterface(iface);
        if constexpr (InheritsNodeBase<T>) {
            if (t.mediaClass() == T::mediaClass()) {
                auto node = T::fromNode(t);
                _interfaceAddedSubject.get_observer().on_next(node);
                _interfaces.emplace(global.id, node);
                return;
            }
        } else {
            _interfaceAddedSubject.get_observer().on_next(t);
            _interfaces.emplace(global.id, t);
        }
    }
}

template <IsInterface T>
void Registry<T>::onGlobalRemoved(std::uint32_t id) {
    if (!_interfaces.contains(id)) {
        return;
    }
    _interfaceRemovedSubject.get_observer().on_next(_interfaces.at(id));
    _interfaces.erase(id);
}

//template class Registry<InterfaceType::Client, SomeStruct>;
//template class Registry<InterfaceType::Device, SomeStruct>;
template class Registry<AudioSink>;
template class Registry<Link>;
template class Registry<Metadata>;
template class Registry<NodeBase>;
template class Registry<StreamOutputAudio>;
//template class Registry<InterfaceType::Module, SomeStruct>;
