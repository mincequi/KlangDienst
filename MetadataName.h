#pragma once

#include <string>

#include <magic_enum.hpp>

enum class MetadataName {
    Default,
    RouteSettings
};

std::string toString(MetadataName name) {
    auto n = std::string(magic_enum::enum_name<>(name));
    n[0] = tolower(n[0]);
    // Separate words with hyphen and make them lowercase
    for (size_t i = 1; i < n.size(); ++i) {
        if (isupper(n[i])) {
            n.insert(i++, "-");
            n[i] = tolower(n[i]);
        }
    }
    return n;
}
