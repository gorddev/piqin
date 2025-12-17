#pragma once

#include "engine/types/Vertex.hpp"
#include <SDL_pixels.h>

namespace geng {
    /** A MiniTransform is a transform with less total information: just position, color, and scale */
    struct FontTransform {
        Vertex pos = {0.f, 0.f, 0.f};
        SDL_Color color;
        float scale;
    };
}
