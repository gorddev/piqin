#include "utilities/BasisDecoder.hpp"
#include <fstream>
#include <iostream>
#include <SDL.h>

#include "Constants.hpp"


// does what the function name says.
SDL_Texture* BasisDecoder::load_ktx2_as_texture(
    SDL_Renderer *renderer, std::string path) {

    // First we grab the file (its in binary, at the end of the file [std::ios::ate])
    std::ifstream file(
        path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "(1) Failed to open file " << path << ".\n";
        return nullptr;
    }

    // Then we get the size of the file.
    size_t size = file.tellg();
    // Go to the start of the file
    file.seekg(0);

    // We then create a vector given the size of our file to read our binary into. (8 bits per)
    std::vector<uint8_t> data(size);
    // Get the array of the vector so we can read into it (we need to reinterpret to read bin)
    auto arr = reinterpret_cast<char *>(data.data());
    file.read(arr, size);

    /* Start using BASISU */
    // Create a transcoder object.
    basist::basisu_transcoder_init();
    basist::ktx2_transcoder transcoder;

    // Init is basically saying "is the metadata & file signature correct"
    if (!transcoder.init(data.data(), data.size())) {
        std::cerr << "(2) Failed to open file " << path << ".\n";
        return nullptr;
    }


    if (!(transcoder.is_uastc() || transcoder.is_etc1s())) {
        printf("ERROR: KTX2 file does NOT contain basis data!\n");
    }

    // If our basis
    // file is empty, we can't do shit.
    if (transcoder.get_levels() < 1) {
        std::cerr << "(3) Failed to open file " << path <<
                ".\n";
        return nullptr;
    }

    // Here we get information about singel images.
    basist::ktx2_image_level_info imgInfo;

    // START Loop {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, LOAD_RENDER_STYLE);
    SDL_Texture* texture = load_ktx2_image_as_texture(
        renderer, data, transcoder, imgInfo, 0);

    // } END Loop

    return texture;
}

// Gets an image number and creates a texture from it
// INPUTS:
// Renderer SDL_Renderer
// data -> bits of the .basis file
// basisu_image_info info -> so we reuse an image object
// imageNum -> number of image in the .basis file
SDL_Texture* BasisDecoder::load_ktx2_image_as_texture(
    SDL_Renderer *renderer,
    const std::vector<uint8_t> &data,
    basist::ktx2_transcoder &transcoder,
    basist::ktx2_image_level_info &info,
    unsigned int imageNum) {

    // {Start loop
    transcoder.get_image_level_info(info, imageNum, 0, 0);

    // width and height of the image
    uint32_t w = info.m_width;
    uint32_t h = info.m_height;

    // now we decode to rgba format with and RGBA buffer
    std::vector<uint8_t> rgba(w * h * 4);
    // We need 32 bits for each rgba value

    // We prep the transcoder to start decoding the data for rgba format
    // Now it's decoding the .ktx2
    // this is where we do it.
    transcoder.start_transcoding();
    bool successey = transcoder.transcode_image_level(
        imageNum,
         0, 0,
         rgba.data(),
         w*h,
         basist::transcoder_texture_format::cTFRGBA32);

    // pray it succeeds
    if (!successey) {
        std::cerr << "Basis: transcoding failed (turn into texture) \n";
        return nullptr;
    }
    // finally the part that we could've just done if we included the SDL2.png library
    // First, we make a basic RGBA32 texture
    SDL_Texture *tex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STATIC,
        // This just means that it's an unchanging texture
        w, h
    );

    // Finally, we upload our rgba data to the texture
    SDL_UpdateTexture(tex, nullptr, rgba.data(), w * 4);
    // this blendmode us have transparent thingies.
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    return tex;
}
