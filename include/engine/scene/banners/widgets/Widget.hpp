#pragma once
#include "engine/scene/banners/BannerEnums.hpp"
#include "engine/scene/banners/BannerBuffer.hpp"
#include "engine/types/positioning/Direction2D.hpp"
#include "engine/types/positioning/Box2D.hpp"


namespace geng {
    /** @brief A widget is one renderable element in a Banner. It can be an image or text. It can also be animated, though implementation is more complicated if you desire that.
     * @detailed. Widgets are an abstract base class and have the following member variables and functions
     * - Each widget has a @code width@endcode and @code height@endcode
     */
    class Widget {
    protected:
        /// Dimensions of Widget: contains the width, height, relative x, and relative y of the Widget. Should not change.
        Box2D dim;
        /// Contains the padding of the widget relative to the border of the banner
        Direction2D padding;
        /// Contains the texture id of the Widget
        int texture_id = -1;
        /// Alignment of the widget
        Align align;
    public:
        virtual ~Widget() = default;

        /// Full constructor for the widget
        Widget(int x, int y, int w, int h, int tex_id)
            : dim(x, y, w, h), padding(0, 0, 0, 0), align(Align::NONE), texture_id(tex_id) {}
        /// Constructor that just concerns size and alignment
        Widget(int w, int h, int tex_id, Align align, Direction2D padding = {0,0,0,0})
            : dim(0, 0, w, h), texture_id(tex_id), padding(padding), align(align){}
        /// To_vertex function. Must override & return number of vertices appended.
        virtual int to_vertex(BannerBuffer& buffer) = 0;
        /// You also must override the change dimension function so you know what the consequences are if you change dimensions. You can put nothing here if you really want to.
        virtual void change_dim(Dim2D dimensions) = 0;
        /// Optional update function if your widget has animated elements or something else exciting.
        virtual void update(LayerState& time) {}

        /// Returns the area of this widget in pixels^2
        [[nodiscard]] int area() const { return dim.w & dim.h; }
        /// Returns the width of this widget
        [[nodiscard]] int get_width() const { return dim.w; }
        /// Returns the height of this widget
        [[nodiscard]] int get_height() const { return dim.h; }
        /// Returns the position of this widget
        [[nodiscard]] Pos2D get_pos() const { return {dim.x, dim.y}; }
        /// Returns the texture id of this widget
        [[nodiscard]] int get_texture_id() const { return texture_id; }
        /// Sets the position of the widget
        void set_pos(const Pos2D pos) { dim.x = pos.x; dim.y = pos.y; }

    };
}
