#pragma once
#include "Widget.hpp"
#include "engine/utilities/Utilities.hpp"

namespace gan {

    /** The WidgetBox is a widget that is just a horizontal line with a color **/
    class WidgetBox : public gan::Widget {
        /// Color of the line
        SDL_Color color;
        /// Vertexes where the WidgetBox should be rendered relative to the Banner
        std::vector<SDL_FPoint> points;
        friend class Banner;
    public:
        /// Creates the horizontal line with thickness, length, vertical padding, horizontal padding, ect.
        WidgetBox(int thickness, int length, SDL_Color color = {gan::max_alpha, gan::max_alpha, gan::max_alpha, gan::max_alpha}, short padding_vert = 0, short padding_hor = 0)
            : Widget(length, thickness, -1, gan::Align::NONE, {padding_vert, padding_vert, padding_hor, padding_hor})
                , color(color) {
            points = box.to_points();

        }
        /// Default destructor
        ~WidgetBox() override = default;

        /// Creates our WidgetBox FPos2D map
        int to_vertex(gan::BannerBuffer& buffer) override {
            buffer.push_back(points[0], color);
            buffer.push_back(points[0 + 2], color);
            buffer.push_back(points[0 + 1], color);
            buffer.push_back(points[0], color);
            buffer.push_back(points[0 + 2], color);
            buffer.push_back(points[0 + 3], color);
            return 6;
        }

        /// Updates our dimension accordingly
        void change_dim(dim2 dimensions) override {
            box.w = dimensions.w;
            box.h = dimensions.h;
            points = box.to_points();
        }

        /// Allows us to set the color
        void set_color(SDL_Color new_color) {
            color = new_color;
        }

        /// Allows us to set the alpha of the widgetbox
        void set_alpha(uint_fast8_t new_alpha) {
            color.a = new_alpha;
        }

        /// Update function does nothing, but allows us to continue overriding.
        void update(gan::LayerState& time) override {};

    };
}
