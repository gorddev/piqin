#include <engine/mods/banners/banner-types/HoverBanner.hpp>

using namespace gfx;

HoverBanner::HoverBanner(gan::vec2 pos, gan::dim2 size, float speed, SDL_Color bg_color, SDL_Color border_color)
    : Banner(pos, size.w, size.h), background(-100, -100, bg_color),
        background_color(bg_color), border_color(border_color),
        fader(speed, true)
{
    // Set up the background and color
    background.set_color(background_color);
    border.set_color(border_color);
    background.set_alpha(0);
    border.set_alpha(0);
    border.change_dim({-100, -100});

    // Adding widgets
    add_widget(&background);
    add_widget(&border);

}

void HoverBanner::update(gan::LayerState &time) {
    // Update each of our widgets
    Banner::update(time);

    // Update our fader
    fader.update(time.get_dt(), time.get_time());

    // then with our new values we update the colors with the target colors
    background.set_alpha(fader.calc_alpha(background_color.a));
    border.set_alpha(fader.calc_alpha(border_color.a));
}

void HoverBanner::on_hover() {
    fader.fade_in();
}

void HoverBanner::on_hover_release() {
    fader.fade_out();
}
