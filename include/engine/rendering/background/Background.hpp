#pragma once
#include <SDL.h>

#include "BackgroundPalette.hpp"
#include "../../core/EngineContext.hpp"

namespace gan {
    /** To be heavily altered/changed. Currently the Background edits every pixel in the background, which is very very inefficient. */
    class Background {
    private:
        // Stores the pixel of each of our rgb values
        std::vector<uint8_t> rgb;
        // Heightmap for dune-like structure.
        std::vector<float> heightMap;
        // Keeps track of the intended color of each pixel
        std::vector<uint8_t> pixelColors;
        // Then tells us what pallete we're on
        int paletteNum = 0;
        // The texture that the background writes to.
        SDL_Texture* background_texture;
        // Keeps track of the engine_context
        EngineContext& world;

        void update_pixels();

    public:
        // Constructor
        explicit Background(SDL_Renderer* renderer, EngineContext& world);
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
}
