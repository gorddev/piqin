#pragma once
#include "../debug-utilities/DebugText.hpp"
#include "engine/input/InputRouter.hpp"
#include "engine/layers/Layer.hpp"
#include "engine/scene/banners/text/Text.hpp"
#include "engine/core/defaults/sysfont.inl"
#include "engine/types/strings/fstring/fstring.hpp"
#include "../debug-utilities/default-debug-syntax-map.hpp"

namespace geng::debug {

    /** The Console can be used to print out text and the like **/
    class Console {
    public:
        Banner topinfo;
        DebugText<800> text;
        Gear* selected = nullptr;

    public:
        explicit Console(InputRouterInit& init)
                : text("debug", sys_font),
                    topinfo({1,0},0,0)
        {
            topinfo.add_widget(&text);
        }

        bool update(Layer*& active_layer, DebugContext& debugger) {
            text.clear();
            geng::str_view view = text.get_fstr_view();
            view << precision<3>() << "debug: " << debugger.is_debug() << "\thitbox: " << debugger.is_hitboxes() << "\tgrid: " << debugger.is_grid()
                << "\tvisible: " << active_layer->scene.is_visible() << "\n"
                << "active_layer: [e]" << active_layer->get_name().cstr() << " [n](" << active_layer->scene.get_id() << ")\n"
                << "mouse selection: " << active_layer->input.mouse.target << "\n";\


            if (selected == nullptr) {
                if (active_layer->input.mouse.target != nullptr)
                    active_layer->input.mouse.target->to_fstring_verbose(view) << "\n";
            }
            else {
                view << selected << "\n";
                selected->to_fstring_verbose(view) << "\n";
            }

            text.update_text(text.get_text().cstr());
            return false;
        }

        void to_vertex(RenderBuffer& buffer) {
            buffer.debug_mode(true);
            topinfo.to_vertex(buffer);
            buffer.debug_mode(false);
        }

        bool get_press(SDL_Scancode key, Layer *&active_layer) {
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
