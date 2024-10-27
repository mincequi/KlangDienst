#pragma once

#include "Interface.h"
#include "InterfaceType.h"
#include "pipewire/keys.h"

#include <rohrkabel/global.hpp>

class NodeBase : public Interface {
public:
    static InterfaceType type() {
        return InterfaceType::Node;
    }

    inline static NodeBase fromInterface(Interface& iface) {
        return NodeBase(iface);
    }

    const std::string& mediaClass() const {
        return _mediaClass;
    }

private:
    NodeBase(Interface& iface) :
        Interface(std::move(iface)) {
        if (props().contains(PW_KEY_MEDIA_CLASS)) _mediaClass = props().at(PW_KEY_MEDIA_CLASS);
    }

    std::string _mediaClass;
};
