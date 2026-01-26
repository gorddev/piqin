#pragma once
#include "engine/debug/debug-utilities/DebugButton.hpp"
#include "engine/input/InputHandler.hpp"


namespace gan::debug {

    /** Holds all of the buttons in the debug panel **/
    class DebugButtonPanel {
    private:
        EngineContext& core;
        /// Contains each of the buttons in the panel.
        std::vector<DebugButton> button_modes;
        /// Mouse acceptor num
        int acceptors = 0;
        /// accepts the camera
        const Camera& cam;

        void add_button_mode(bool& bind, char symbol) {
            button_modes.emplace_back(
                vec2{cam.get_width() - debug_button_size.w/2.f, debug_button_size.w/2.f + (debug_button_size.h*button_modes.size())},
                bind, core.get_font(0), symbol, debug_button_size);
        }
    public:
        explicit DebugButtonPanel(EngineContext& core, const Camera& cam) : core(core), cam(cam)
        {
            button_modes.reserve(6);
            add_button_mode(core.debugger.debug_mode, '~');
            add_button_mode(core.debugger.grid, '#');
            add_button_mode(core.debugger.hitboxes, 'X');
            add_button_mode(core.debugger.verbose, 'V');
            add_button_mode(core.debugger.change_layer, '>');
            add_button_mode(core.debugger.pause_layer, 'P');
        }

        void notify_screen_resolution_change(dim2 new_res) {
            for (int i = 0; i < button_modes.size(); i++)
                button_modes[i].set_pos({new_res.w - debug_button_size.w/2.f, debug_button_size.h/2.f + debug_button_size.h*i});
        }

        void to_vertex(RenderBuffer& buffer) {
            for (auto& i: button_modes) {
                i.to_vertex(buffer);
            }
        }

        void add_mouse_acceptors(InputHandler& button_manager) {
            for ( ; acceptors < button_modes.size(); acceptors++)
                button_manager.add_mouse_acceptor(&button_modes[acceptors]);
        }

        void notify_update() {
            for (auto& i: button_modes)
                i.update();
        }
    };

}
