#include "engine/debug/console/DebugInfoBanner.hpp"

#include "engine/mods/banners/text/Text.hpp"

namespace gan::debug {

    DebugInfoBanner::DebugInfoBanner(EngineContext& core)
        : Banner({1.0, 0.0}, 0, 0), info({1, 0}, 0, 0),
          text(" ", core.get_font(0)),
          core(core),
          fps_tracker() {
        info.add_widget(&text);
        text.resize_internal_string(1200);
    }

    void DebugInfoBanner::update(Layer*& active_layer, DebugContext& debugger) {
        text.clear();
        str_view view = text.get_fstr_view();

        fps_tracker.push(1000.f/core.get_dt());

        float fps = 0; int i = 0;
        for (auto& it: fps_tracker) {
            fps += (it);
            i ++;
        }
        fps /= (i*5);
        fps = static_cast<int>(fps);
        fps *= 5;

        if (debugger.is_recovery()) {
            view << "recovery: true";
        }
        else if (debugger.is_verbose()) {
            // print general debug information
            view << precision<3>() << "debug ([t]verbose[n])"
                 << "\thtb: " << debugger.is_hitboxes()
                 << "\tgrid: " << debugger.is_grid()
                << "fps: " << fps << "Hz";


            if (active_layer != nullptr) {
                view << "\tvis: " << active_layer->scene.is_visible() << "\n"
                     << "active_layer: [e]" << active_layer->get_name().c_str()
                     << " (" << active_layer->scene.get_id() << ")[n]\n"
                     << "mpos: {" << active_layer->input.mouse.pos.x << ",\t" << active_layer->input.mouse.pos.y << "}\t" <<
                         "mouse selection: " << active_layer->input.mouse.target << " ";

                if (selected == nullptr) {
                    if (active_layer->input.mouse.target != nullptr)
                        active_layer->input.mouse.target->to_fstring_verbose(view) << "\n";
                }
                else {
                    view << "selected: " << selected << "\n";
                    selected->to_fstring_verbose(view) << "\n";
                }
            }
        }
        else {
            view << precision<3>() << "debug\t";

            if (active_layer != nullptr) {
                view << "active layer: [e]" << active_layer->get_name().c_str()
                     << " (" << active_layer->scene.get_id() << ")[n]\tfps: " << fps << "Hz\n";

                if (selected == nullptr) {
                    if (active_layer->input.mouse.target != nullptr)
                        active_layer->input.mouse.target->to_fstring(view) << "\n";
                }
                else {
                    view << "selected: " << selected << "\n";
                    selected->to_fstring(view) << "\n";
                }
            }
        }
        text.update_text(text.get_text().c_str());
    }

    void DebugInfoBanner::to_vertex(RenderBuffer& buffer) {
        info.to_vertex(buffer);
    }

    bool DebugInfoBanner::get_press(SDL_Scancode key, Layer*& active_layer) {
        glog::note << "get_press" << glog::endlog;
        if (key == SDL_SCANCODE_RETURN) {
            if (selected != nullptr)
                selected = nullptr;
            else if (active_layer != nullptr) {

                selected = active_layer->input.mouse.target;
            }
        }
        return false;
    }

} // namespace gan::debug