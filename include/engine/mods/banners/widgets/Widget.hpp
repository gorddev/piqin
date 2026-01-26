#pragma once
#include "engine/mods/banners/BannerEnums.hpp"
#include "engine/mods/banners/BannerBuffer.hpp"
#include "engine/types/positioning/Direction2D.hpp"
#include "engine/types/positioning/box2.hpp"
#include "engine/layers/layer-subobjects/LayerCore.hpp"

namespace gan {
    /**
     * @class Widget
     * @brief Abstract base class for all banner UI widgets.
     *
     * The Widget class represents a drawable and alignable UI element inside
     * a Banner system. It encapsulates geometry, texture ownership, alignment,
     * padding, visibility, and update behavior, while deferring rendering and
     * layout consequences to derived classes.
     *
     * This class is intended to be subclassed. Derived widgets must provide
     * implementations for vertex generation and dimension-change handling.
     *
     * ------------------
     *
     * Member Vars
     *
     * @code Box2D box@endcode
     * Stores the dimensions and relative position of the widget.
     * - @code x, y@endcode: relative position
     * - @code w, h@endcode: width and height
     * This value generally should not be mutated arbitrarily.
     *
     * @code int texture_id@endcode
     * Texture identifier associated with the widget.
     * A value of -1 indicates no texture. Banners can only have one texture id at a time
     *
     * @code Direction2D padding@endcode
     * Padding applied relative to the banner border during alignment.
     *
     * @code Align alignment@endcode
     * Alignment mode used when positioning the widget within a banner.
     *
     * @code bool visible@endcode
     * Visibility flag. Invisible widgets should not emit vertex data.
     *
     * -------------------------------------------------------------------------
     *
     * @code Widget(int x, int y, int w, int h, int tex_id)@endcode
     * Full constructor specifying explicit position, size, and texture.
     *
     * @code Widget(int w, int h, int tex_id, Align align, Direction2D padding)@endcode
     * Constructor specifying size, texture, alignment, and optional padding.
     * Positioning is deferred until alignment.
     *
     * @code virtual ~Widget()@endcode
     * Virtual destructor to allow safe deletion via base pointer.
     *
     * -------------------------------------------------------------------------
     *
     * @code virtual int to_vertex(BannerBuffer& buffer)@endcode
     * Appends this widget's vertex data to a banner buffer.
     * Must be overridden by derived classes.
     * Returns the number of vertices appended.
     *
     * @code virtual void change_dim(Dim2D dimensions)@endcode
     * Called when widget dimensions are changed.
     * Derived classes must define how dimension changes affect internal state.
     *
     * @code virtual void align(Dim2D banner_dim)@endcode
     * Called by the Banner to align the widget based on banner dimensions.
     * Optional override.
     *
     * @code virtual void update(LayerState& time)@endcode
     * Optional update hook for animated or time-dependent widgets.
     *
     * -------------------------------------------------------------------------
     * @section widget_accessors Accessors & Mutators
     *
     * @code int area() const@endcode
     * Returns the area of the widget in pixels squared.
     *
     * @code int get_width() const@endcode
     * Returns the width of the widget.
     *
     * @code int get_height() const@endcode
     * Returns the height of the widget.
     *
     * @code Pos2D get_pos() const@endcode
     * Returns the current position of the widget.
     *
     * @code int get_texture_id() const@endcode
     * Returns the widget's texture ID.
     *
     * @code void set_pos(const Pos2D pos)@endcode
     * Sets the widget's position.
     *
     * @code void set_visible(bool visibility)@endcode
     * Sets the widget's visibility.
     *
     * @code bool is_visible()@endcode
     * Returns whether the widget is visible.
     *
     * @code void toggle_visible()@endcode
     * Toggles the widget's visibility state.
     *
     * -------------------------------------------------------------------------
     * @code
     * class ImageWidget : public Widget {
     * public:
     *     using Widget::Widget;
     *
     *     int to_vertex(BannerBuffer& buffer) override {
     *         // append vertices
     *         return 6;
     *     }
     *
     *     void change_dim(Dim2D dim) override {
     *         // handle resize
     *     }
     * };
     * @endcode
     *
     * @note This class is abstract and cannot be instantiated directly.
    */
    class Widget {
    protected:
        /// Dimensions of Widget: contains the width, height, relative x, and relative y of the Widget. Should not change.
        box2 box;
        /// Contains the texture id of the Widget
        int texture_id = -1;
        /// Contains the padding of the widget relative to the border of the banner
        Direction2D padding;
        /// Alignment of the widget
        Align alignment;
        /// visiblity of the widget
        bool visible = true;
    public:
        virtual ~Widget() = default;

        /// Full constructor for the widget
        Widget(int x, int y, int w, int h, int tex_id)
            : box(x, y, w, h), padding(0, 0, 0, 0), alignment(Align::NONE), texture_id(tex_id) {}
        /// Constructor that just concerns size and alignment
        Widget(int w, int h, int tex_id, Align align, Direction2D padding = {0,0,0,0})
            : box(0, 0, w, h), texture_id(tex_id), padding(padding), alignment(align){}
        /// To_vertex function. Must override & return number of vertices appended.
        virtual int to_vertex(BannerBuffer& buffer) = 0;
        /// You also must override the change dimension function so you know what the consequences are if you change dimensions. You can put nothing here if you really want to.
        virtual void change_dim(dim2 dimensions) {};
        /// Called by the Banner to properly align the widget. Gives the dimensions of the banner
        virtual void align(dim2 banner_dim) {}
        /// Optional update function if your widget has animated elements or something else exciting.
        virtual void update(LayerState& time) {}

        /// Returns the area of this widget in pixels^2
        [[nodiscard]] int area() const { return box.w & box.h; }
        /// Returns the width of this widget
        [[nodiscard]] int get_width() const { return box.w; }
        /// Returns the height of this widget
        [[nodiscard]] int get_height() const { return box.h; }
        /// Returns the position of this widget
        [[nodiscard]] pos2 get_pos() const { return {box.x, box.y}; }
        /// Returns the texture id of this widget
        [[nodiscard]] int get_texture_id() const { return texture_id; }
        /// Sets the position of the widget
        void set_pos(const pos2 pos) { box.x = pos.x; box.y = pos.y; }
        // <><><> Visibility <><><>
        /// sets visiblity
        void set_visible(bool visiblity) { visible = visiblity; }
        /// gets visibility
        [[nodiscard]] bool is_visible() { return visible; }
        /// toggles visiblity
        void toggle_visible() { visible = !visible; }
    };
}
