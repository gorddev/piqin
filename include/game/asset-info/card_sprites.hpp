#pragma once

#include <engine/gan_std.hpp>

namespace init {
    /// Frame table used by small cards
    inline const gan::FrameTable card_table = {
        0, 0, 20, 29, 13, 56, 1
    };
    /// Frame table used to highlight cards
    inline const gan::FrameTable card_highlight = {
        {
            // Highlighter
            {
                {320, 32, 26, 36}
            },
            // Selector
            {
                {352, 32, 16, 16},
                {352, 48, 16, 16}
            }
        }
    };
    //448, 32, 32, 48, 1, 1
}