#pragma once
#include "engine/mods/banners/Banner.hpp"
#include "engine/mods/banners/text/Text.hpp"
#include "engine/mods/banners/widgets/WidgetBorder.hpp"
#include "engine/mods/banners/widgets/WidgetBox.hpp"

namespace gan::debug {

    const dim2 debug_button_size = {20, 20};

    class DebugButton : public Banner {
    private:
        /// Symbol displayed on the button
        char symbol = '#';
        /// Bound boolean
        bool& bind;
        /// Border of the button
        WidgetBorder border;
        /// Background of the button
        WidgetBox background;
        /// Text of the button
        Text text;

    public:
        DebugButton(vec2 pos, bool& bind, Font& font, char symbol = '#',  dim2 size = debug_button_size)
            : Banner(pos, size.w, size.h), symbol(symbol), bind(bind), border(1),
                background(-100, -100, {0,0,0,120}), text("[c:green]", font) {

            add_background(&background);
            add_background(&border);
            text.resize_internal_string(10);
            text.set_pos({size.w/2 - text.get_font()->get_width(symbol)/2, size.h/2 - text.get_font()->get_height(symbol)/2});
            text.update_text((fstring<11>("[c:green]") << symbol).c_str());
            add_widget(&text);
            DebugButton::on_hover_release();
        }

        // on click
        void on_click(pos2) override {
            bind = !bind;
            if (bind) {
                text.update_text((fstring<11>("[c:green]") << symbol).c_str());
                on_hover();
            }
            else {
                text.update_text((fstring<11>("[c:red]") << symbol).c_str());
                on_hover();
            }
        }

        void on_hover() override {
            if (bind)
                border.set_color(color_green);
            else
                border.set_color(color_red);
        }
        void on_hover_release() override {
            if (bind)
                border.set_color({50,140,50,gan::max_alpha});
            else
                border.set_color({140,50,50,gan::max_alpha});
        }

        void update() {

        }

        void to_vertex(RenderBuffer& buffer) override {
            Banner::to_vertex(buffer);
        }
    };
}
