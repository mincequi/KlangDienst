#pragma once

#include "Interface.h"
#include "InterfaceType.h"

class Metadata : public Interface {
public:
    static InterfaceType type() {
        return InterfaceType::Metadata;
    }

    inline static Metadata fromInterface(Interface& iface) {
        return Metadata(iface);
    }

private:
    Metadata(Interface& iface) :
        Interface(std::move(iface)) {
    }
};
