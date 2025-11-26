#include "../../../include/scene/render/background-utilities/Background.hpp"
#include "Constants.hpp"
#include <iostream>
#include <cstdint>

// Lets us generate a perlin map at startup:
#define STB_PERLIN_IMPLEMENTATION
#include <vector>

#include "generics/Vertex.hpp"

Background::Background(SDL_Renderer* renderer) {
    // First we make the background texture we write to.
    background_texture = SDL_CreateTexture (
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        scene::width,
        scene::height
    );

    // Then we resize our vector in accordance with the number of pixels we'll need.
    rgb.resize(scene::width * scene::height * 4);
    create_heightMap();
    pixelColors = std::vector<uint8_t>(scene::width * scene::height);

    // Then, we set up a default Palette to use.
    addPalette(defaultBackgroundPalette);
    paletteNum = 0;
    setPalette(0);

    for (int i = 0; i < 200; i++)
        update(20);
}

Background::~Background() {
    SDL_DestroyTexture(background_texture);
}

SDL_Texture *Background::get_texture() const {
    return background_texture;
}

void Background::update(int dt) {
    void* pixels;
    int pitch;

    SDL_LockTexture(background_texture, nullptr, &pixels, &pitch);
    if (rgb.size() != scene::width * scene::height * 4)
        rgb.resize(scene::width * scene::height * 4);

    //update_pixels();

    for (int y =0; y < scene::height; y++) {
        memcpy(
            (uint8_t*)pixels + y * pitch,
            &rgb[(y * scene::width) * 4],   // each pixel is 4 bytes
            scene::width * 4
        );
    }

    SDL_UnlockTexture(background_texture);
}

static uint8_t toColor(int c) {
    if (c < 0) c = 0;
    else if (c > 255) c = 255;
    return c;
}

#define PIX_ALL 7
#define PIX_FIRST 4
#define PIX_SECOND 2
#define PIX_THIRD 1
void Background::update_pixels() {
    uint8_t range = palettes[paletteNum].range;
    uint8_t speed = palettes[paletteNum].speed;
    pixelColors[0] = 0;

    if (scene::frame % speed == 0) {
        for (int i = 0; i < rgb.size(); i+=4) {
            // This is our switch
            if (random() % palettes[paletteNum].chaos == 0){
                short t1, t2, t3;
                t1 = palettes[paletteNum].colors[pixelColors[i/4]*4];
                t2 = palettes[paletteNum].colors[pixelColors[i/4]*4 + 1];
                t3 = palettes[paletteNum].colors[pixelColors[i/4]*4 + 2];
                signed char mod = (rgb[i+3] & PIX_ALL) ? 1 : -1;
                rgb[i] = toColor(rgb[i] + (mod*speed));
                rgb[i+1] = toColor(rgb[i+1] + (mod*speed));
                rgb[i+2] = toColor(rgb[i+2] + (mod*speed));
                if (rgb[i] < t1 - range || rgb[i]==0)
                    rgb[i+3] |= PIX_FIRST;
                else if (rgb[i] > t1 + range || rgb[i]==255)
                    rgb[i+3] ^= PIX_FIRST;

                if (rgb[i+1] < t2 - range || rgb[i+1]==0)
                    rgb[i+3] |= PIX_SECOND;
                else if (rgb[i+1] > t2 + range || rgb[i+1]==255)
                    rgb[i+3] ^= PIX_SECOND;

                if (rgb[i+2] < t3 - range || rgb[i+2]==0)
                    rgb[i+3] |= PIX_THIRD;
                else if (rgb[i+2] > t3 + range || rgb[i+2]==255)
                    rgb[i+3] ^= PIX_THIRD;

                if (i < 100)
                    std::cerr << rgb[i] + 0 << ", " << rgb[i+1] + 0 << ", " << rgb[i+2] + 0 << ", " << rgb[i+3] +0<< std::endl;
            }
        }
    }
}

void Background::addPalette(const BackgroundPalette& bp) {
    palettes.push_back(bp);
}

// sets a new Palette
void Background::setPalette(int newPalette) {
    // If we're already using this palette it probably means startup so we refresh colors

    if (newPalette == paletteNum) {
        for (int i =0; i < pixelColors.size(); i++) {
            pixelColors[i] = int((palettes[paletteNum].numColors) * heightMap[i]);
        }
        int range = palettes[paletteNum].range;
        for (int i = 0; i < rgb.size(); i+=4) {
            rgb[i] = palettes[paletteNum].colors[pixelColors[i/4]*4];
            rgb[i+1] = palettes[paletteNum].colors[pixelColors[i/4]*4 + 1];
            rgb[i+2] = palettes[paletteNum].colors[pixelColors[i/4]*4 + 2];
            rgb[i+3] = 254 + (random() %2);
        }


    }
    // Return error if the palette is not loaded
    if (newPalette >= palettes.size()) {
        std::cerr << "Err::Background::setPalette " << newPalette << " not found." << std::endl;
        return;
    }
    paletteNum = newPalette;
}

void Background::create_heightMap() {
    // creates a height map for dune-like structures
    heightMap.resize(scene::width * scene::height);

    float scaley = 10.0f / scene::height;
    float scalex = 20.0f / scene::width;

    float max = 0;
    float min = 0;
    Vertex source(1,1,1);

    // Now we take cross product with respect to the gradient of f
    for (int y = 0; y < scene::height; y++) {
        for (int x = 0; x < scene::width; x++) {
            float sx = (x-(scene::width/2)) * scalex;
            float sy = (y-(scene::height/2)) * scaley;

            //float h = 0.5*sinf(sy/4.0f - sx/2.0f) + 0.25*(sinf(sy-sx+cos(0.5f*sy-sx)))+sinf(0.2f*sx)+cosf(0.1f*sy);

            float gradient_x = -0.25*cosf(sy/4.0f - 0.5*sx) + 0.25*(0.5*sinf(sy-sx)-0.1*cosf(sy-sx)) + 0.1*cosf(sx/5.0f);
            float gradient_y = 0.125*cosf(sy/4.0f - 0.5*sx)+0.25*(0.5*cosf(sy-sx)-0.02*cosf(sy-sx))-0.05*cosf(sy/10.0f);
            float gradient_z = 1;
            Vertex gradient(gradient_x, gradient_y, gradient_z);


            // Here we just use this equation to get a map of the sand region.
            float h = gradient*source;

            if (h > max) max = h;
            else if (h < min) min = h;
            // add it to our height map
            heightMap[y * scene::width + x] = h;
        }
    }

    float range = max - min;
    for (float &h : heightMap)
        h = (h - min) / range;
}


