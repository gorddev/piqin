#pragma once
#include <utility>
#include <SDL_render.h>
#include "engine/utilities/image-info/IMG_Info.hpp"

namespace gan {
    /** Creates a base texture to use with info on it **/
    struct Texture {
        IMG_Info info = {"null", 0, 0};
        SDL_Texture* texture = nullptr;;
        Texture() = default;
        Texture(const IMG_Info &info, SDL_Texture* texture)
            : info(info), texture(texture) {}
        Texture(const Texture& t) = default;
    };
}
