#pragma once

#include <iostream>
#include <ostream>

#include "Font.hpp"

namespace geng {

    /** Manages all fonts so you can easily print fonts to either the canvas or the UI */
    class FontList {
        std::vector<Font> fonts;

    public:
        FontList() = default;
        /// Must call add_fonts before game loop.
        void add_fonts(const std::vector<Font>& new_fonts) {
            for (auto& f : new_fonts) {
                fonts.emplace_back(f);
            }
        }
        /// grabs a font
        Font& at(int id) {
            if (id >= fonts.size() || id < 0) {
                std::cerr << "ERR: Font " << id << "doesn't exist. Using sysfont.\n";
                id = 0;
            }
            return fonts.at(id);
        }
        /// grabs a font with the [] operator
        Font& operator[](int id) {
            return at(id);
        }
    };

}
