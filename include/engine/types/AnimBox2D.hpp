#pragma once

#include <SDL_render.h>
#include "../core/gears/GearFlag.hpp"
#include "external/vector.hpp"

namespace geng {

    /**
     * @brief Describes what happens after each frame ends
     * - @code GAnim::IDLE@endcode › Stop at the end of the animation
     * - @code GAnim::RESET@endcode › Reset the animation to the default_animation
     * - @code GAnim::CONTINUE@endcode › Move to the next frame in memory
     * - @code GAnim::REPEAT@endcode › Repeat the current animation
     */
    enum class GAnimType : uint8_t {
        IDLE = 0,
        RESET = 1,
        CONTINUE = 2,
        REPEAT = 3
    };

    inline const char* to_string(GAnimType anim) {
        if (anim == GAnimType::IDLE)
            return "IDLE";
        if (anim == GAnimType::RESET)
            return "RESET";
        if (anim == GAnimType::CONTINUE)
            return "CONTINUE";
        if (anim == GAnimType::REPEAT)
            return "REPEAT";
        return "null";
    }

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
