#pragma once
#include <SDL.h>
#include <string>
#include <utility>

#include "engine/scene/initializer/Texture.hpp"

namespace geng {

    /**
     * Decodes Various filetypes into textures, and gets their information.
     */
    class IMGDecoder {
    public:
        // Extracts the header information from a png.
        static IMG_Info PNG_Info(const std::string& path);
        // New function signature for loading any image format supported by SDL_image
        static Texture load_image_as_texture(
            SDL_Renderer *renderer, std::string path);
    };
}
