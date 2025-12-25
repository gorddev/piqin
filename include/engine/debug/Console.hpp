#pragma once
#include "engine/input/InputRouter.hpp"
#include "engine/layers/Layer.hpp"
#include "engine/scene/banners/text/Text.hpp"
#include "../defaults/sysfont.inl"
#include "../types/strings/fstring/fstring.hpp"
#include "debug-utilities/default-debug-syntax-map.hpp"

namespace geng::debug {

    /** The Console can be used to print out text and the like **/
    class Console : public InputRouter {
    public:
        Banner topinfo;
        Text<800> text;
        Gear* selected = nullptr;

    public:
        explicit Console(InputRouterInit& init)
                : InputRouter(init), text("debug", sys_font, &geng_default_debug_syntax_map), topinfo({10,0},0,0) {
            text.get_fstr_view() << "debug: " << core.debugger.is_debug();
            topinfo.add_widget(&text);
        }

        bool update(Layer*& active_layer) override {
            text.clear();
            geng::str_view view = text.get_fstr_view();
            view << "debug: " << core.debugger.is_debug() << "\thitbox: " << core.debugger.is_hitboxes() << "\tgrid: " << core.debugger.is_grid()
                    << "\tvisible: " << active_layer->scene.is_visible() << "\n"
                << "active_layer: [e]" << active_layer->get_name().cstr() << " [n](" << active_layer->scene.get_id() << ")^c:white^\n"
                << "mouse selection: " << active_layer->input.mouse.target << "\n";


            if (selected == nullptr) {
                if (active_layer->input.mouse.target != nullptr)
                    active_layer->input.mouse.target->to_fstring(view) << "\n";
            }
            else {
                view << selected << "\n";
                selected->to_fstring(view) << "\n";
            }
            return false;
        }

        void to_vertex(RenderBuffer& buffer) {
            if (core.debugger.is_debug()) {
                buffer.debug_mode(true);
                topinfo.to_vertex(buffer);
                buffer.debug_mode(false);
            }
        }

        bool get_press(SDL_Scancode key, Layer *&active_layer) override {
            if (key == SDL_SCANCODE_RETURN) {
                if (selected != nullptr)
                    selected = nullptr;
                else {
                    selected = active_layer->input.mouse.target;
                }
            }
            return false;
        }
    };
}
