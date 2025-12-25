#pragma once

#include "engine/scene/world/WorldProperties.hpp"
#include "engine/types/positioning/Box2D.hpp"

namespace geng {
    struct TileGrid {
        /// Contains all the tiles in a single gameworld.
        const gch::vector<uint16_t>& tiles;
        /// Width of one gameworld level
        int width;
        /// Height of one gameworld level
        int height;
        /// Tilesize of a gameworld level
        int tileSize;

        TileGrid(GameLevel* level, int tilesize)
            : tiles(level->extract_physics_data()),
            width(level->width), height(level->height),
            tileSize(tilesize) {}

        /// Gets the tile at a location
        uint16_t at(int tx, int ty) const {
            if (tx < 0 || ty < 0 || tx >= width || ty >= height)
                return 0;
            return tiles[ty * width + tx];
        }

        /// Turns the tile into a box.
        Box2D tile_box(int tx, int ty) const {
            return {
                tx * tileSize,
                ty * tileSize,
                tileSize,
                tileSize
            };
        }

        /// Returns a buffer of vertices with the hitboxes of each box
        gch::vector<SDL_FPoint> display_hitboxes(uint16_t thickness) const {
            gch::vector<SDL_FPoint> buffer;
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    if (at(i,j) != 0) {
                        gch::vector<SDL_FPoint> points = tile_box(i, j).to_vertex_hitbox(thickness);
                        buffer.insert(buffer.end(), points.begin(), points.end());
                    }
                }
            }
            return buffer;
        }
    };
}
