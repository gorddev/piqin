#pragma once
#include <vector>

#include "engine/scene/banners/widgets/Widget.hpp"
#include "engine/types/Gear.hpp"

namespace geng {

    /** Banner is how you make text and all types of other shit that you want to make and display on the screen hooray hooray **/
    class Banner : public Gear {
    private:
        /// Contains all the background widgets
        std::vector<Widget*> background_widgets;
        /// Contains all the regular widgets.
        std::vector<Widget*> widgets;
        /// The buffer you pass to each widget, because they don't get to see the regular renderbuffer.
        BannerBuffer banner_buffer;

    public:
        /// Default constructor
        Banner(Vertex pos, uint16_t width, uint16_t height);

        /// To_vertex that lets the banner be renderable.
        void to_vertex(RenderBuffer &buffer) override;

        /// This is a must-have function if we don't wnat Banner to be an abstract base class
        float z_index() const override;

        /// Calling update on our Banner
        void update(LayerTime& time);

        /// Add a background widget
        bool add_background(Widget* w);
        /// Add a normal widget
        bool add_widget(Widget* w);
        /// Get banner texture id
        int get_texture_id() const;
        /// Access widgets
        const std::vector<Widget*>& get_widgets() const;

    private:
        bool add_widget_internal(Widget* w);

        void on_hover() override;
        void on_click() override;
    };

}