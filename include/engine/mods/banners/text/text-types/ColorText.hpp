#pragma once

#include "engine/core/defaults/syntax-maps/NullSyntaxMap.hpp"
#include "engine/mods/banners/text/Text.hpp"

namespace gan {

    /// Create a piece of text with a specific color. Will be mono_color automatically
    class ColorText : public Text {
    private:
        SDL_Color color;
    public:
        ColorText(const char text[], Font& font, SDL_Color color, Align alignment = Align::LEFT)
            : Text(text, font, alignment, &null_syntax_map), color(color)
        {
            for (auto& v: this->vertices) {
                v.color = color;
            }
        }
    };
}
