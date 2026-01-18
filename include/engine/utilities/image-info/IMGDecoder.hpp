#pragma once
#include <SDL.h>

#include <utility>

#include "engine/scene/font/Font.hpp"
#include "engine/scene/initializer/Texture.hpp"
#include "engine/types/strings/hstring/hstring.hpp"

namespace gan {
    /**
     * Decodes Various filetypes into textures, and gets their information.
     */
    class IMGDecoder {
    public:
        // Extracts the header information from a png.
        static IMG_Info PNG_Info(const char path[]);
        // New function signature for loading any image format supported by SDL_image
        static Texture load_image_as_texture(
            SDL_Renderer *renderer, hstring path);

        static Texture load_font_as_texture(SDL_Renderer *renderer,
                                     Font &font,
                                     hstring path);

        static Texture create_sysfont_texture(
            SDL_Renderer *renderer);
    };
}
