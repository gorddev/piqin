#pragma once

#include <SDL_rect.h>
#include <vector>
#include "EngineEnums.hpp"

namespace geng {
    /**
    * @brief Largely a temporary type used for converting rectangular coordinates into Frame objects or vertices for vertex rendering.
    * @details You primary use the @code to_frame@endcode function to convert to a frame object. Frame objects can then take Vertex data and create render output from it.
    */
    struct Quad {
        int x, y, w, h;
        float duration;
        GAnimType anim;
        /// Constructor for a quad. Can hold useful info for
        Quad(int x, int y, int w, int h,
            float duration = 0.f, GAnimType anim = GAnimType::IDLE);

        /// Converts a quad into it's vertex points
        std::vector<SDL_FPoint> to_vert_points(int texW, int texH) const;
    };
}
