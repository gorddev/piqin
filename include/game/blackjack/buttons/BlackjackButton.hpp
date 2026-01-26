#pragma once
#include "engine/mods/banners/Banner.hpp"
#include "engine/mods/banners/text/Text.hpp"

namespace bj {

    class BlackjackButton : public gan::Banner {
    public:

        struct ButtonInit {
            gan::Font& font;
            SDL_Color color;
            SDL_Color border_color;
        };
        BlackjackButton(gan::vec2 pos, const char text[], ButtonInit init)
            : Banner(pos, 20, 30)
        {
            add_widget(new gan::Text<20>(text, init.font, gan::Align::CENTER));
        }
    };
}
