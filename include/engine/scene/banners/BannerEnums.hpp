#pragma once
#include <cstdint>

namespace geng {

    /** Determines the alignment of an item on a widget. **/
    enum class Align : uint8_t {
        NONE = 0,
        LEFT = 1 << 0,
        RIGHT = 1 << 1,
        CENTER = 1 << 2,
        TOP = 1 << 3,
        BOTTOM = 1 << 4,
    };

    // Boilerplate enum shit
    inline bool operator ==(Align a, Align b) {
        return static_cast<uint8_t>(a) == static_cast<uint8_t>(b);
    }
    inline Align operator &(Align a, Align b) {
        return static_cast<Align>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }
    inline Align operator |(Align a, Align b) {
        return static_cast<Align>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }
    inline void operator &=(Align& a, Align b) {
        a = static_cast<Align>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }
    inline void operator |=(Align& a, Align b) {
        a = static_cast<Align>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }
}
