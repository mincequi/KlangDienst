#pragma once

#include <pipewire/client.h>

enum class InterfaceType {
    Unknown,
    Client,
    Core,
    Device,
    Factory,
    Link,
    Metadata,
    Module,
    Node,
    Port,
    Registry,
};
