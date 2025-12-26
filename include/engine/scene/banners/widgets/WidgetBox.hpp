#pragma once
#include "Widget.hpp"

namespace geng {

    /** The WidgetBox is a widget that is just a horizontal line with a color **/
    class WidgetBox : public geng::Widget {
        /// Color of the line
        SDL_Color color;
        /// Vertexes where the WidgetBox should be rendered relative to the Banner
        gch::vector<SDL_FPoint> points;
    public:
        /// Creates the horizontal line with thickness, length, vertical padding, horizontal padding, ect.
        WidgetBox(int thickness, int length, SDL_Color color = {255, 255, 255, 255}, short padding_vert = 0, short padding_hor = 0)
            : Widget(length, thickness, -1, geng::Align::CENTER, {padding_vert, padding_vert, padding_hor, padding_hor})
                , color(color) {
            points = box.to_points();
        }
        /// Default destructor
        ~WidgetBox() override = default;

        /// Creates our WidgetBox FPos2D map
        int to_vertex(geng::BannerBuffer& buffer) override {
            buffer.push_back(points[0], color);
            buffer.push_back(points[2], color);
            buffer.push_back(points[1], color);
            buffer.push_back(points[0], color);
            buffer.push_back(points[2], color);
            buffer.push_back(points[3], color);
            return 6;
        }

        /// Updates our dimension accordingly
        void change_dim(Dim2D dimensions) override {
            box.w = dimensions.w;
            box.h = dimensions.h;
            points = box.to_points();
        }

        /// Update function does nothing, but allows us to continue overriding.
        void update(geng::LayerState& time) override {};

    };
}
