#pragma once
#include "engine/types/Gear.hpp"
#include "engine/utilities/Utilities.hpp"

namespace geng {

    /** @brief Holds basic information for a mouse to be used by the Inputhandler*
     * @details Mouse has the following member variables
     */
    struct Mouse {

    public:
        /// Coordinates of the mouse
        int x = 0, y = 0;
        /// Change in position of mouse frame to frame
        float dx = 0.f, dy = 0.f;
        /// Target of mouse action
        Gear* target = nullptr;
        /// Whether the mouse is down or not.
        bool down = false;
        /// Reference to the vector of mouse recievers.
        std::vector<Gear*>& mouse_recievers;

        /// This constructor links the mouse to the vector it oversees.
        explicit Mouse(std::vector<Gear*>& mouse_recievers)
            : mouse_recievers(mouse_recievers) {}

        /// Does things to a target if the mouse is hovering over something right now.
        void on_click() {
            down = true;
            if (target != nullptr) {
                target->_engine_flagger(GFlag::clicked);
                target->on_click();
                if (target->is_draggable()) {
                    target->_engine_flagger(GFlag::dragged);
                }
            }
        }

        /// Does things on click release
        void on_click_release() {
            down = false;
            if (target != nullptr) {
                target->_engine_deflagger(GFlag::clicked);
                target->_engine_deflagger(GFlag::dragged);
                target->on_click_release();
            }
        }

        /// Pops the current target for if the target becomes invalid
        void pop_target() {
            if (target != nullptr) {
                flush_target();
            }
        }

        /// Called when the mouse is moved by some distance.
        void on_movement(SDL_Point pos, float dx, float dy, EngineContext& world) {
            // First update our cursor position
            x = pos.x;
            y = pos.y;
            this->dx = dx;
            this->dy = dy;

            // If we currently have a target
            if (target != nullptr) {
                // If we're dragging, we update position.
                if (target->is_dragged()) {
                    target->t.pos += {dx, dy, 0.f};
                    target->t.snap_to_scene(world);
                    return;
                }
                // If we're no longer contained within the target we need to update
                if (!gutils::contained_within({x,y}, target->t)) {
                    target->on_hover_release();
                    flush_target();
                }
                // If everything's chill, we just exit.
                else
                    return;
            }
            // Search for a new target.
            for (int i = static_cast<int>(mouse_recievers.size()) - 1; i >= 0; i--) {
                if (gutils::contained_within({x,y}, mouse_recievers[i]->t)) {
                    target = mouse_recievers[i];
                    target->on_hover();
                }
            }
        }

        void flush_target() {
            target->_engine_deflagger(GFlag::clicked);
            target->_engine_deflagger(GFlag::dragged);
            target->_engine_deflagger(GFlag::hovered);
            target = nullptr;
        }
    };
}
