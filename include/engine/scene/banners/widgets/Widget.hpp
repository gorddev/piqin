#pragma once
#include "../BannerEnums.hpp"
#include "engine/scene/banners/BannerBuffer.hpp"
#include "engine/scene/layers/LayerTime.hpp"
#include "engine/types/positioning/Point.hpp"
#include "engine/types/positioning/Vertex.hpp"

namespace geng {
    /** @brief A widget is one renderable element in a Banner. It can be an image or text. It can also be animated, though implementation is more complicated if you desire that.
     * @detailed. Widgets are an abstract base class and have the following member variables and functions
     * - Each widget has a @code width@endcode and @code height@endcode
     */
    class Widget {
    protected:
        /// Dimensions of Widget: contains the width, height, relative x, and relative y of the Widget. Should not change.
        Box dim;
        /// Contains the padding of the widget relative to the border of the banner
        Directional padding;
        /// Contains the texture id of the Widget
        int texture_id = -1;
        /// Alignment of the widget
        Align align;
    public:
        /// Full constructor for the widget
        Widget(int x, int y, int w, int h, int tex_id)
            : dim(x, y, w, h), padding(0, 0, 0, 0), align(Align::NONE), texture_id(tex_id) {}
        /// Constructor that just concerns size and alignment
        Widget(int w, int h, int tex_id, Align align, Directional padding = {0,0,0,0})
            : dim(0, 0, w, h), texture_id(tex_id), padding(padding), align(align){}

        /// To_vertex function. Must override & return number of vertices appended.
        virtual int to_vertex(BannerBuffer& buffer, Vertex& banner_pos) = 0;
        /// Optional update function if your widget has animated elements or something else exciting.
        virtual void update(LayerTime& time) {}

        /// Returns the area of this widget in pixels^2
        [[nodiscard]] int area() const { return dim.w & dim.h; }
        /// Returns the width of this widget
        [[nodiscard]] int get_width() const { return dim.w; }
        /// Returns the height of this widget
        [[nodiscard]] int get_height() const { return dim.h; }
        /// Returns the position of this widget
        [[nodiscard]] Point get_pos() const { return {dim.x, dim.y}; }
    };
}
