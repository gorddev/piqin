#pragma once

#include "Font.hpp"
#include "../../debug/geng_debug.hpp"

namespace gan {

    /** Manages all fonts so you can easily print fonts to either the canvas or the UI */
    class FontList {
        std::vector<Font> fonts;

    public:
        FontList() {
            fonts.reserve(3);
        }

        /// Must call add_font before game loop.
        Font* create_font(const hstring& path, uint16_t spacing, uint16_t pt) {
            fonts.emplace_back(path, pt, spacing);
            return &fonts.back();
        }

        Font* instantiate_font(const Font& f) {
            fonts.emplace_back(f);
            return &fonts.back();
        }

        /// Adds a texture id to the back.

        /// grabs a font
        Font& at(int id)  {
            if (id >= static_cast<int>(fonts.size()) || id < 0) {
                glog::warn << "ERR: Font " << id << "doesn't exist. Using sysfont.\n" << glog::endlog;
                id = 0;
            }
            return fonts.at(id);
        }
        /// grabs a font with the [] operator
        Font& operator[](int id) {
            return at(id);
        }
        /// Returns the size of the fontlist
        [[nodiscard]] int size() {
            return static_cast<int>(fonts.size());
        }
    };

}
