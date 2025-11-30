#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include "basisu_transcoder/basisu_transcoder.h"
#include "basisu_transcoder/basisu_containers.h"

class BasisDecoder {
public:
    BasisDecoder() = delete;
    [[nodiscard]] static SDL_Texture* load_ktx2_as_texture(SDL_Renderer* renderer, std::string path);
    [[nodiscard]] static SDL_Texture* load_ktx2_image_as_texture(
        SDL_Renderer *renderer,
        const std::vector<uint8_t> &data,
        basist::ktx2_transcoder &transcoder,
        basist::ktx2_image_level_info &info,
        unsigned int imageNum);
};
