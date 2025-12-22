#pragma once
#include "Tileset.hpp"
#include "engine/rendering/RenderBuffer.hpp"

namespace geng {

    /// The TileBuffer allows tile IDs to be linked to a tileset
    class TileBuffer {
    private:
        RenderBuffer& rend_buffer;
        Tileset& tileset;
    public:
        TileBuffer(RenderBuffer& buffer, Tileset& tileset) : rend_buffer(buffer), tileset(tileset) {}

        void push_back(int tile_id, Pos2D tileOffset) {
            if (tile_id > tileset.size())
                tile_id = 0;
            tileset[tile_id].to_vertex(rend_buffer, (tileOffset).to_f_point());
        }

        int get_tile_size() {
            return tileset.tile_size();
        }

    };
}
