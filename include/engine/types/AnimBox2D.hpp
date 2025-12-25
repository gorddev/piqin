#pragma once

#include <SDL_render.h>
#include "EngineEnums.hpp"
#include "external/vector.hpp"

namespace geng {
    /**
    * @brief Largely a temporary type used for converting rectangular coordinates into Frame objects or vertices for FPos2D rendering.
    * @details You primary use the @code to_frame@endcode function to convert to a frame object. Frame objects can then take FPos2D data and create render output from it.
    */
    struct AnimBox2D {
        float duration;
        uint16_t x, y, w, h;
        short next_anim = -1;
        GAnimType anim;
        AnimBox2D() = default;
        /// Constructor for a quad. Can hold useful info for
        AnimBox2D(int x, int y, int w, int h,
            float duration = 0.f, GAnimType anim = GAnimType::IDLE, short next_anim = -1);

        /// Converts a quad into it's corresponding FPos2D tex_points
        gch::vector<SDL_FPoint> to_vert_points(int texW, int texH) const;

        /// Converts a quad into a string!
        std::string to_string() const;
    };
}
