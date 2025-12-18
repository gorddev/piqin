#pragma once
#include "Widget.hpp"

namespace geng {

    /** The HLine is a widget that is just a horizontal line with a color **/
    class HLine final : public Widget {
        /// Color of the line
        SDL_Color color;
    public:
        /// Creates the horizontal line with thickness, length, vertical padding, horizontal padding, ect.
        HLine(int thickness, int length, short padding_vert = 0, short padding_hor = 0, SDL_Color color = {255, 255, 255, 255})
            : Widget(thickness, length, -1, Align::CENTER, {padding_vert, padding_vert, padding_hor, padding_hor})
                , color(color) {}



        void update(LayerTime& time) override {};

    };
}
