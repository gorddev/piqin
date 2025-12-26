#pragma once

#include "Font.hpp"
#include "../../debug/geng_debug.hpp"

namespace geng {

    /** Manages all fonts so you can easily print fonts to either the canvas or the UI */
    class FontList {
        gch::vector<Font> fonts;

    public:
        FontList() = default;

        /// Must call add_font before game loop.
        int add_font(const Font& new_font) {
            fonts.emplace_back(new_font);
            return fonts.size() - 1;
        }
        /// Must call add_fonts before game loop.
        void add_fonts(gch::vector<Font>& new_fonts) {
            for (auto& f : new_fonts) {
                fonts.emplace_back(f);
            }
        }
        /// grabs a font
        Font& at(int id) {
            if (id >= fonts.size() || id < 0) {
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
