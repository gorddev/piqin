#pragma once
#include "engine/mods/banners/Banner.hpp"
#include "engine/mods/banners/widgets/WidgetBorder.hpp"
#include "engine/mods/banners/widgets/WidgetBox.hpp"
#include "engine/utilities/Fader.hpp"

namespace gfx {

    /// Lights up on hover with a background and border of choice color
    class HoverBanner : public gan::Banner {
        /// Background of the banner
        gan::WidgetBox background;
        /// Border of the banner
        gan::WidgetBorder border;
        /// Target color of the background
        SDL_Color background_color;
        /// Target color of the border
        SDL_Color border_color;
        /// Fader for the HoverBanner
        gutils::Fader fader;

    public:
        /// Required constructor for a hover banner
        HoverBanner(gan::vec2 pos, gan::dim2 size, float speed,
            SDL_Color bg_color = {gan::max_alpha, gan::max_alpha, gan::max_alpha, 50},
            SDL_Color border_color = {gan::max_alpha, gan::max_alpha, gan::max_alpha, 130});

        /// Update function
        void update(gan::LayerState& time) override;

        /// Fades in on hover
        void on_hover() override;
        /// Fades out on hover release
        void on_hover_release() override;
    };
}
