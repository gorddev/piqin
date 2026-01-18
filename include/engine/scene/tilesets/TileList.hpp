#pragma once

#include <fstream>
#include "Tileset.hpp"
#include "../../debug/geng_debug.hpp"

namespace gan {

    /** Manages all fonts so you can easily print fonts to either the canvas or the UI */
    class TileList {
        gch::vector<Tileset> tilesets;

    public:
        TileList() = default;

        int add_tileset(const Tileset& new_tileset) {
            tilesets.emplace_back(new_tileset);
            return size() - 1;
        }
        /// Must call add_tilesets
        void add_tilesets(gch::vector<Tileset>& new_tilesets) {
            for (auto& t : new_tilesets) {
                tilesets.emplace_back(t);
            }
        }
        /// grabs a font
        Tileset& at(int id) {
            if (id >= static_cast<int>(tilesets.size()) || id < 0) {
                glog::err.src("tilesets") << "ERR: Tileset " << id << "doesn't exist.\n";
                id = 0;
            }
            return tilesets.at(id);
        }
        /// grabs a font with the [] operator
        Tileset& operator[](int id) {
            return at(id);
        }
        /// returns the size of the tileset
        int size() const {
            return static_cast<int>(tilesets.size());
        }
    };

}
