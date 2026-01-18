#pragma once
#include "Widget.hpp"

namespace gan {

    /** The WidgetBox is a widget that is just a horizontal line with a color **/
    class WidgetBorder : public gan::Widget {
        /// Color of the line
        SDL_Color color;
        /// Vertexes where the WidgetBox should be rendered relative to the Banner
        gch::vector<SDL_FPoint> points;
        /// Thickness
        uint16_t thickness;
    public:
        /// Creates the horizontal line with thickness, length, vertical padding, horizontal padding, ect.
        WidgetBorder(uint16_t thickness = 2, SDL_Color color = {255, 255, 255, 255})
            : Widget(-100, -100, -1, Align::NONE)
                , thickness(thickness), color(color) {}
        /// Default destructor
        ~WidgetBorder() override = default;

        /// Creates our WidgetBox FPos2D map
        int to_vertex(gan::BannerBuffer& buffer) override {
            for (auto& point : points) {
                buffer.push_back(point, color);
            }
            return 6;
        }

        /// Updates our dimension accordingly
        void change_dim(Dim2D dimensions) override {
            box.w = dimensions.w;
            box.h = dimensions.h;
            points = box.to_hitbox(thickness);
        }

        /// changes the thickness of the line
        void change_thickness(const uint16_t thick) {
            thickness = thick;
        }

        /// changes the color of the WidgetBorder
        void set_color(const SDL_Color col) {
            color = col;
        }

        /// Update function does nothing, but allows us to continue overriding.
        void update(gan::LayerState& time) override {};

    };
}
