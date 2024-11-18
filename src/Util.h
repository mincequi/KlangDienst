#pragma once

#include <rohrkabel/node/info.hpp>
#include <rohrkabel/node/node.hpp>

static std::string name(const pipewire::node_info& node) {
    if (node.props.contains("node.name"))
        return node.props.at("node.name");
    else
        return "";
}

static std::string name(const pipewire::node& node) {
    auto props = node.props();
    if (props.contains("node.name"))
        return props.at("node.name");
    else
        return "";
}
