#pragma once
#include "../../types/strings/fstring/fstring.hpp"
#include <SDL_rect.h>
#include "engine/debug/geng_debug.hpp"

#define GENG_PATH_MAX_LEN 50
namespace gan {
    /**
     * Used for returning information to the user about the image
     */
    struct IMG_Info {
        fstring<GENG_PATH_MAX_LEN> filename = "null";
        uint32_t w;
        uint32_t h;
        IMG_Info(const char filen[], uint32_t w, uint32_t h)
            : filename(filen), w(w), h(h) {}

        /// Returns the emplaced white point on the texture.
        SDL_FPoint get_white_point() const {
            return {(w - 0.5f)/w, (h - 0.5f)/h};
        }

        gan::str_view& to_fstring(gan::str_view& buffer) {
            return buffer <<  "Filename: " << filename.wrap() << " w: " << w << " h: " << h;
        }
    };
}
