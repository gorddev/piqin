#include "engine/utilities/image-info/IMGDecoder.hpp"
#include <SDL2/SDL_image.h>
#include <fstream>

#include "engine/debug/logging/LogSource.hpp"
#include "engine/scene/initializer/Texture.hpp"
#include "engine/types/strings/hstring/hstring.hpp"

using namespace geng;

static bool is_PNG(geng::hstring path) {
    return true;
}

static long to_pow_2(const int x) {
    long i = 1;
    for (; i < x; i*=2){}
    return i;
}

IMG_Info IMGDecoder::PNG_Info(const char path[]) {
    if (!is_PNG(path)) {
        glog::err << "File " << path << "is not a png\n";
    }
    // Creates a file we read with binary from
    std::ifstream file(path, std::ios::binary);
    if (!file)
        glog::err.src("Is_PNG") << "FATAL: Image" << path << " does not exist.";
    uint8_t header[24];
    // Read from the file
    file.read(reinterpret_cast<char*>(header), 24);
    // check against the signature:
    static const uint8_t sig[8] =
        {137, 80, 78, 71, 13, 10, 26, 10};
    // actually we'll just use memcmp instead
    if (std::memcmp(header, sig, 8) != 0) {
        glog::err << "FATAL: " << path << " File signature does not match .png format";
        abort();
    }
    // create a small function lambda that grabs the bits
    auto read_int = [](const uint8_t* p) {
        return static_cast<uint32_t>(to_pow_2((static_cast<uint32_t>(p[0]) << 24) |
                (static_cast<uint32_t>(p[1]) << 16) |
                (static_cast<uint32_t>(p[2]) << 8) |
                (static_cast<uint32_t>(p[3]) << 0)));
    };
    // create our image object with an initializer list.
    return {path, read_int(header + 16), read_int(header + 20)};
}


// Note: We rename load_ktx2_as_texture to the new function name load_image_as_texture
Texture IMGDecoder::load_image_as_texture(
    SDL_Renderer *renderer, hstring path) {


    SDL_Surface* src = IMG_Load(path.cstr());
    src = SDL_ConvertSurfaceFormat(src, SDL_PIXELFORMAT_RGBA8888, 0);

    int new_w = to_pow_2(src->w);
    int new_h = to_pow_2(src->h);
    SDL_Surface* padded = SDL_CreateRGBSurfaceWithFormat(
        0,
        new_w,
        new_h,
        32,
        SDL_PIXELFORMAT_RGBA8888
    );

    SDL_Rect dst = {0, 0, src->w, src->h};
    SDL_BlitSurface(src, nullptr, padded, &dst);

    Uint32 white = SDL_MapRGBA(padded->format, 255,255,255,255);
    ((Uint32*)padded->pixels)[(new_h - 1) * new_w + (new_w - 1)] = white;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, padded);
    SDL_FreeSurface(src);
    SDL_FreeSurface(padded);

    if (tex == nullptr) {
        glog::err << "Failed to load image file " << path << " using SDL_image.\n";
        glog::err << "SDL_image Error: " << IMG_GetError() << "\n";
        abort();
    }

    // This blend mode setting is often still required for transparency
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    return {PNG_Info(path), tex};
}

