#pragma once

#include "Interface.h"
#include "InterfaceType.h"

class Link : public Interface {
public:
    static InterfaceType type() {
        return InterfaceType::Link;
    }

    inline static Link fromInterface(Interface& iface) {
        return Link(iface);
    }

private:
    Link(Interface& iface) :
        Interface(std::move(iface)) {
    }
};
