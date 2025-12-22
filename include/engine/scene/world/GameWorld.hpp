#pragma once
#include <string>
#include <vector>

#include "WorldProperties.hpp"
#include "engine/rendering/Camera.hpp"
#include "engine/scene/tilesets/TileBuffer.hpp"

namespace geng {
    /// GameWorld contains all the tiles and the entire game world through which you play the game.
    struct GameWorld {
        uint64_t magic = 0;
        uint16_t num_levels;
        std::vector<GameLevel> levels;
    public:
        /// Returns the first level
        GameLevel* get_first_level();
        /// Searches for a specific level
        GameLevel* search_for_level(int x, int y);

        /// Returns the width of the engine_context
        int worldWidth();
        /// Returns the height of the engine_context.
        int worldHeight();

        /// Renders everything within the gameLevel
        void render_world(geng::TileBuffer& buffer, geng::Camera& cam);

        /// Prints out the gameworld:
        std::string to_string() const;
    };
}