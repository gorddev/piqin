#pragma once
#include <SDL_rect.h>

namespace gengine {
    struct FontChar {
        SDL_Rect loc;
        int pen;
        int x;
        int y;
    };
}
