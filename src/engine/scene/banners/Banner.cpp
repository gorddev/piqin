#include "engine/scene/banners/Banner.hpp"
#include "engine/types/positioning/Box2D.hpp"
#include <algorithm>

#include "engine/debug/geng_debug.hpp"

using namespace geng;

/// Default constructor
Banner::Banner(FPos2D pos, uint16_t width, uint16_t height)
    : Gear({pos, width, height}),
      banner_buffer(t) {}

/// To_vertex that lets the banner be renderable.
void Banner::to_vertex(RenderBuffer &buffer) {
    // Set up our banner_buffer
    banner_buffer._set_white_point(buffer.get_white_point());
    banner_buffer._set_shadow_bank(&buffer.get_shadow_bank());

    for (auto& b : background_widgets)
        if (b->is_visible())
            b->to_vertex(banner_buffer);
    for (auto& w : widgets)
        if (w->is_visible())
            w->to_vertex(banner_buffer);

    buffer.push_back(banner_buffer._get_vertex_buffer());

    // Clear out our banner buffer
    banner_buffer._clear_buffer();
}

/// Calling update on our Banner
void Banner::update(LayerState& time) {
    for (auto& i : background_widgets)
        i->update(time);
    for (auto& w : widgets)
        w->update(time);
}

/// Add a background widget
bool Banner::add_background(Widget* w) {
    return add_widget_internal(w);
}

/// Add a normal widget
bool Banner::add_widget(Widget* w) {
    return add_widget_internal(w);
}

/// Get banner texture id
int Banner::get_texture_id() const {
    return texture_id;
}

void Banner::set_pos(FPos2D pos) {
    t.pos = pos;
}

/// Access widgets
gch::vector<Widget*>& Banner::get_widgets() {
    return widgets;
}

bool Banner::add_widget_internal(Widget* w) {
    if (!w) return false;

    // Update banner texture id if not set yet
    if (texture_id < 0) {
        texture_id = w->get_texture_id();
    }

    // Reject widget if texture_id doesn't match
    if (w->get_texture_id() != texture_id) {
        glog::warn << "Widget rejected: texture_id mismatch ("
                  << w->get_texture_id() << " != " << texture_id << ")\n";
        return false;
    }

    if (w->get_width() < 0) {
        float mod = std::min(100.f, 0.f + std::abs(w->get_width())) / 99.9f;
        w->change_dim({static_cast<int>(t.w * mod), w->get_height()});
    }

    if (w->get_height() < 0) {
        float mod = std::min(100.f, 0.f + std::abs(w->get_height())) / 99.9f;
        w->change_dim({w->get_width(), static_cast<int>(t.h * mod)});
    }

    widgets.push_back(w);
    return true;
}
