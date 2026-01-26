#pragma once

#include <engine/gan_anim.hpp>

namespace init {
    /// Frame table used by small cards
    inline const gan::FrameTable card_table = {
        0, 0, 32, 32, 13, 56
    };
    /// Frame table used to highlight cards
    inline const gan::FrameTable card_highlight = {
        454, 41, 22, 31, 1, 1
    };
}