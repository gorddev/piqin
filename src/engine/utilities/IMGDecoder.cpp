#include "../../../include/engine/utilities/image-info/IMGDecoder.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <vector>

#include "engine/scene/initializer/Texture.hpp"

using namespace geng;

static bool is_PNG(const std::string& path) {
    if (path.length() < 4)
        return false;
    // Grabs the extension
    std::string extension = path.substr(path.length() - 4);
    // Transforms the extension to lowercase
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    // Returns whether our extension is PNG or not
    return (extension == ".png");
}

IMG_Info IMGDecoder::PNG_Info(const std::string &path) {
    if (!is_PNG(path)) {
        std::cerr << "File " + path + "is not a png\n";
        abort();
    }

    // Creates a file we read with binary from
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "FATAL: Image" + path + " does not exist." << std::endl;;
        abort();
    }

    uint8_t header[24];
    // Read from the file
    file.read(reinterpret_cast<char*>(header), 24);
    // check against the signature:
    static const uint8_t sig[8] =
        {137, 80, 78, 71, 13, 10, 26, 10};

    // actually we'll just use memcmp instead
    if (std::memcmp(header, sig, 8) != 0) {
        std::cerr << "FATAL: "+ path+" File signature does not match .png format" << std::endl;
        abort();
    }

    // create a small function lambda that grabs the bits
    auto read_int = [](const uint8_t* p) {
        return ((static_cast<uint32_t>(p[0]) << 24) |
                (static_cast<uint32_t>(p[1]) << 16) |
                (static_cast<uint32_t>(p[2]) << 8) |
                (static_cast<uint32_t>(p[3]) << 0)) + 2;
    };

    // create our image object with an initializer list.
    return {path, read_int(header + 16), read_int(header + 20)};
}


// Note: We rename load_ktx2_as_texture to the new function name load_image_as_texture
Texture IMGDecoder::load_image_as_texture(
    SDL_Renderer *renderer, std::string path) {

    // original: w × h
    // padded:   (w+1) × (h+1)

    SDL_Surface* src = IMG_Load(path.c_str());
    src = SDL_ConvertSurfaceFormat(src, SDL_PIXELFORMAT_RGBA32, 0);

    SDL_Surface* padded = SDL_CreateRGBSurfaceWithFormat(
        0,
        src->w + 2,
        src->h + 2,
        32,
        SDL_PIXELFORMAT_RGBA32
    );

    SDL_Rect dst = {0, 0, src->w, src->h};
    SDL_BlitSurface(src, nullptr, padded, &dst);

    Uint32 white = SDL_MapRGBA(padded->format, 255,255,255,255);
    ((Uint32*)padded->pixels)[(padded->h - 2) * padded->w + (padded->w - 2)] = white;
    ((Uint32*)padded->pixels)[(padded->h - 2) * padded->w + (padded->w - 1)] = white;
    ((Uint32*)padded->pixels)[(padded->h - 1) * padded->w + (padded->w - 2)] = white;
    ((Uint32*)padded->pixels)[(padded->h - 1) * padded->w + (padded->w - 1)] = white;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, padded);
    SDL_FreeSurface(src);
    SDL_FreeSurface(padded);

    if (tex == nullptr) {
        std::cerr << "Failed to load image file " << path << " using SDL_image.\n";
        std::cerr << "SDL_image Error: " << IMG_GetError() << "\n";
        abort();
    }

    // This blend mode setting is often still required for transparency
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    return {PNG_Info(path), tex};
}

