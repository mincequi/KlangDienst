#pragma once

#include <magic_enum.hpp>

enum class MediaClass {
    AudioSink,
    StreamOutputAudio,
};

inline std::string toString(MediaClass mediaClass) {
    auto c = std::string(magic_enum::enum_name(mediaClass));
    for (auto i = 1; i < c.size(); ++i) {
        if (std::isupper(c[i])) {
            c.insert(i++, 1, '/');
        }
    }
    return c;
}
