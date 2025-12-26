#include "engine/debug/console/DebugInfoBanner.hpp"

#include "engine/scene/banners/text/Text.hpp"
#include "engine/core/defaults/sysfont.inl"

namespace geng::debug {

    DebugInfoBanner::DebugInfoBanner()
        : Banner({1.0, 0.0}, 0, 0),
          info({1, 0}, 0, 0),
          text(" ", sys_font)
    {
        info.add_widget(&text);
    }

    void DebugInfoBanner::update(Layer*& active_layer, DebugContext& debugger) {
        text.clear();
        geng::str_view view = text.get_fstr_view();

        if (debugger.is_recovery()) {
            view << "recovery: true";
        }
        else if (debugger.is_verbose()) {
            // print general debug information
            view << precision<3>() << "debug ([t]verbose[n])"
                 << "\thitbox: " << debugger.is_hitboxes()
                 << "\tgrid: " << debugger.is_grid();

            if (active_layer != nullptr) {
                view << "\tvis: " << active_layer->scene.is_visible() << "\n"
                     << "active_layer: [e]" << active_layer->get_name().cstr()
                     << " (" << active_layer->scene.get_id() << ")[n]\n"
                     << "mouse selection: " << active_layer->input.mouse.target << "\n";

                if (selected == nullptr) {
                    if (active_layer->input.mouse.target != nullptr)
                        active_layer->input.mouse.target->to_fstring_verbose(view) << "\n";
                }
                else {
                    view << "Selected: " << selected << "\n";
                    selected->to_fstring_verbose(view) << "\n";
                }
            }
        }
        else {
            view << precision<3>() << "debug\t";

            if (active_layer != nullptr) {
                view << "active layer: [e]" << active_layer->get_name().cstr()
                     << " (" << active_layer->scene.get_id() << ")[n]\n";

                if (selected == nullptr) {
                    if (active_layer->input.mouse.target != nullptr)
                        active_layer->input.mouse.target->to_fstring(view) << "\n";
                }
                else {
                    view << "\tselected: " << selected << "\n";
                    selected->to_fstring_verbose(view) << "\n";
                }
            }
        }

        text.update_text(text.get_text().cstr());
    }

    void DebugInfoBanner::to_vertex(RenderBuffer& buffer) {
        info.to_vertex(buffer);
    }

    bool DebugInfoBanner::get_press(SDL_Scancode key, Layer*& active_layer) {
        if (key == SDL_SCANCODE_RETURN) {
            if (selected != nullptr)
                selected = nullptr;
            else if (active_layer != nullptr)
                selected = active_layer->input.mouse.target;
        }
        return false;
    }

} // namespace geng::debug