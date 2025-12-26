#pragma once
#include "engine/core/defaults/sysfont.inl"
#include "engine/scene/banners/Banner.hpp"
#include "engine/scene/banners/text/Text.hpp"
#include "engine/scene/banners/widgets/WidgetBorder.hpp"
#include "engine/scene/banners/widgets/WidgetBox.hpp"

namespace geng::debug {


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
        Text<12> text;
    public:
        DebugButton(FPos2D pos, bool& bind, char symbol = '#',  Dim2D size = {11, 11})
            : Banner(pos, size.w, size.h), symbol(symbol), bind(bind), text("[c:green]", sys_font),
                border(1), background(-100, -100, {0,0,0,120}) {
            add_background(&background);
            add_background(&border);
            text.get_fstr_view() << "[c:green]" << symbol;
            text.set_pos({5 - sys_font.get_width(symbol)/2, 6 - sys_font.get_height(symbol)/2});
            text.update_text(text.get_fstr_view().cstr());
            add_widget(&text);
            DebugButton::on_hover_release();
        }

        // on click
        void on_click(Pos2D pos) override {
            bind = !bind;
            glog::dev << "on_click" << glog::endlog;
            if (bind) {
                text.update_text((fstring<11>("[c:green]") << symbol).cstr());
                on_hover();
            }
            else {
                text.update_text((fstring<11>("[c:red]") << symbol).cstr());
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
                border.set_color({50,140,50,255});
            else
                border.set_color({140,50,50,255});
        }

        void update() {

        }
    };
}
