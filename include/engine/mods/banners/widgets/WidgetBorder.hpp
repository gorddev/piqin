#pragma once
#include "Widget.hpp"

namespace gan {

    /** The WidgetBox is a widget that is just a horizontal line with a color **/
    class WidgetBorder : public gan::Widget {
        /// Vertexes where the WidgetBox should be rendered relative to the Banner
        std::vector<SDL_FPoint> points;
        /// Color of the line
        SDL_Color color;
        /// Thickness
        uint16_t thickness;
    public:
        /// Creates the horizontal line with thickness, length, vertical padding, horizontal padding, ect.
        WidgetBorder(uint16_t thickness = 2, SDL_Color color = {gan::max_alpha, gan::max_alpha, gan::max_alpha, gan::max_alpha})
            : Widget(-100, -100, -1, Align::NONE)
                , color(color), thickness(thickness) {}
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
        void change_dim(dim2 dimensions) override {
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

        /// changes the alpha of the WidgetBorder
        void set_alpha(const uint_fast8_t new_alpha) {
            color.a = new_alpha;
        }

        /// Update function does nothing, but allows us to continue overriding.
        void update(gan::LayerState& time) override {};

    };
}
