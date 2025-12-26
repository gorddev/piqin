#pragma once
#include "engine/debug/debug-utilities/DebugButton.hpp"
#include "engine/input/InputHandler.hpp"


namespace geng::debug {

    /** Holds all of the buttons in the debug panel **/
    class DebugButtonPanel {
    private:
        EngineContext& core;
        /// Contains each of the buttons in the panel.
        gch::vector<DebugButton> button_modes;
        /// Mouse acceptor num
        int acceptors = 0;

        void add_button_mode(bool& bind, char symbol) {
            button_modes.emplace_back(
                FPos2D{core.get_width() - 6.f, 5 + (11.0f*button_modes.size())},
                bind, symbol, Dim2D{11, 11});
        }
    public:
        explicit DebugButtonPanel(EngineContext& core) : core(core)
        {
            button_modes.reserve(6);
            add_button_mode(core.debugger.debug_mode, '~');
            add_button_mode(core.debugger.grid, '#');
            add_button_mode(core.debugger.hitboxes, 'X');
            add_button_mode(core.debugger.verbose, 'V');
            add_button_mode(core.debugger.change_layer, '>');
            add_button_mode(core.debugger.pause_layer, 'P');
        }

        void notify_screen_resolution_change(Dim2D new_res) {
            for (int i = 0; i < button_modes.size(); i++)
                button_modes[i].set_pos({new_res.w - 6.f, 5 + 11.5f*i});
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
