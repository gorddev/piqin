#pragma once

#include "Tile.hpp"

namespace geng {

    /** Contains all the tiles that exist from one tileset. **/
    class Tileset {
    private:
        int texture_id = -1;
        std::vector<Tile> tiles;
        short tile_s;
    public:
        Tileset(int tex_W, int tex_H, short tile_size) : tile_s(tile_size) {
            tiles.reserve(tex_W * tex_H / (tile_size*tile_size));
            for (int h = 0; h < tex_H; h+= tile_size) {
                for (int w = 0; w < tex_W; w+= tile_size) {
                    tiles.emplace_back(Box2D(w, h, tile_size, tile_size), tile_size, tex_W, tex_H);
                }
            }
        }

        int size() const {
            return tiles.size();
        }

        short tile_size() const {
            return tile_s;
        }

        Tile& operator[](int index) {
            return tiles[index];
        }

        void set_texture_id(int id) {
            texture_id = id;
        }

        [[nodiscard]] int get_texture_id() const {
            return texture_id;
        }

        void render_tileset(RenderBuffer& buffer) {
            SDL_FPoint pos = {0, 20};
            for (auto& i: tiles) {
                i.to_vertex(buffer, pos);
                pos.x += tile_s;
            }
        }
    };
}
