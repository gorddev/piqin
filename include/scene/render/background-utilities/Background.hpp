#pragma once
#include <SDL.h>
#include <vector>

#include "BackgroundPalette.hpp"


class Background {
private:
    // Stores the pixel of each of our rgb values
    std::vector<uint8_t> rgb;
    // Heightmap for dune-like structure.
    std::vector<float> heightMap;
    // Keeps track of the intended color of each pixel
    std::vector<uint8_t> pixelColors;
    // Keeps track of the BackgroundPalletes
    std::vector<BackgroundPalette> palettes;
    // Then tells us what pallete we're on
    int paletteNum = 0;
    // The texture that the background writes to.
    SDL_Texture* background_texture;
    // Keeps track of all the Palettes it can use.

    void update_pixels();

public:
    // Constructor
    explicit Background(SDL_Renderer* renderer);
    // Destructor
    ~Background ();
    // Texture getter
    SDL_Texture* get_texture() const;
    // Updater
    void update(int dt);

    // Palette management
    void addPalette(const BackgroundPalette& bp);
    void setPalette(int newPalette);

    void create_heightMap();
};
