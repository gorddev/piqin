#pragma once

#include "engine/scene/banners/widgets/Widget.hpp"
#include "../../core/gears/Gear.hpp"

namespace gan {

    /** Banner is how you make text and all types of other shit that you want to make and display on the screen hooray hooray **/
    class Banner : public Gear {
    private:
        /// Contains all the background widgets
        gch::vector<Widget*> background_widgets;
        /// Contains all the regular widgets.
        gch::vector<Widget*> widgets;
        /// The buffer you pass to each widget, because they don't get to see the regular renderbuffer.
        BannerBuffer banner_buffer;

    public:
        /// Default constructor
        Banner(FPos2D pos, uint16_t width, uint16_t height);

        /// To_vertex that lets the banner be renderable.
        void to_vertex(RenderBuffer &buffer) override;

        /// Calling update on our Banner
        void update(LayerState& time);
        /// Add a background widget
        bool add_background(Widget* w);
        /// Add a normal widget
        bool add_widget(Widget* w);
        /// Get banner texture id
        int get_texture_id() const;
        /// Sets the banner position
        virtual void set_pos(FPos2D pos);
        /// Access widgets
        gch::vector<Widget*>& get_widgets();

    private:
        /// Adds a widget interally
        bool add_widget_internal(Widget* w);
    };

}