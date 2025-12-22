#pragma once

#include <iostream>
#include <ostream>

#include "Tileset.hpp"

namespace geng {

    /** Manages all fonts so you can easily print fonts to either the canvas or the UI */
    class TileList {
        std::vector<Tileset> tilesets;

    public:
        TileList() = default;
        /// Must call add_tilesets
        void add_tilesets(const std::vector<Tileset>& new_tilesets) {
            for (auto& t : new_tilesets) {
                tilesets.emplace_back(t);
            }
        }
        /// grabs a font
        Tileset& at(int id) {
            if (id >= tilesets.size() || id < 0) {
                std::cerr << "ERR: Tileset " << id << "doesn't exist.\n";
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
            return tilesets.size();
        }
    };

}
