#pragma once
#include <utility>

#include "engine/utilities/image-info/IMG_Info.hpp"

namespace geng {
    /** Creates a base texture to use with info on it **/
    struct Texture {
        IMG_Info info;
        SDL_Texture* texture;
        Texture(IMG_Info info, SDL_Texture* texture)
            : info(std::move(info)), texture(texture) {}
        Texture(const Texture& t) = default;
    };
}
