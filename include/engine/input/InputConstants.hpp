#pragma once

namespace gengine {
    inline short GENG_buffer = 400;

    enum class GENG_Input : uint8_t {
        NONE = 0,
        UP = 1,
        DOWN = 2,
        LEFT = 3,
        RIGHT = 4,
        SELECT = 5,
        SELECT_ALT = 6,
        BACK = 7,
        MODIFY = 8,
        PAUSE = 9
    };
}