#pragma once

#include <rohrkabel/global.hpp>

namespace pw = pipewire;

class Interface {
public:
    inline std::uint32_t id() const {
        return _id;
    }

    inline const pw::spa::dict& props() const {
        return _props;
    }

    inline static Interface fromGlobal(const pw::global& global) {
        return Interface(global.id, global.props);
    }

private:
    Interface(std::uint32_t id, const pw::spa::dict& props) :
        _id(id),
        _props(props) {}

    std::uint32_t _id;
    pw::spa::dict _props;
};
