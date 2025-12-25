#pragma once
#include "engine/types/Gear.hpp"
#include "engine/utilities/Utilities.hpp"
#include "engine/rendering/Camera.hpp"

namespace geng {

    /** @brief Holds basic information for a mouse to be used by the Inputhandler*
     * @details Mouse has the following member variables
     */
    struct Mouse {

    public:
        /// Coordinates of the mouse
        Pos2D pos;
        /// Change in position of mouse frame to frame
        FPos2D dpos;
        /// Attatchment point of object relative to center of mouse
        FPos2D relpos = {0, 0};
        /// Target of mouse action
        Gear* target = nullptr;
        /// Whether the mouse is down or not.
        bool down = false;
        /// Reference to the vector of mouse recievers.
        gch::vector<Gear*>& mouse_recievers;
        /// Gets access to the camera
        const Camera& cam;

        /// This constructor links the mouse to the vector it oversees.
        explicit Mouse(gch::vector<Gear*>& mouse_recievers, const Camera& cam)
            : mouse_recievers(mouse_recievers), cam(cam) {}

        /// Does things to a target if the mouse is hovering over something right now.
        void on_click() {
            down = true;
            if (target != nullptr) {
                target->_engine_flagger(GFlag::clicked);
                target->on_click();
                if (target->is_draggable()) {
                    relpos.x = pos.x - target->t.pos.x;
                    relpos.y = pos.y - target->t.pos.y;
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
        void on_movement(SDL_Point new_pos, float dx, float dy, LayerContext& scene) {
            // First we need to sort by z-index first.
            std::sort(mouse_recievers.begin(), mouse_recievers.end(), [](const Gear* a, const Gear* b) {
                return a->z_index > b->z_index;
            });
            // First update our cursor position
            pos = {new_pos.x, new_pos.y};
            dpos = {dx, dy};

            // If we currently have a target
            if (target != nullptr) {
                // If we're dragging, we update position.
                if (target->is_dragged()) {
                    target->t.pos.x = pos.x - relpos.x;
                    target->t.pos.y = pos.y - relpos.y;
                    target->t.snap_to_scene(scene);
                    return;
                }
            }
            target = nullptr;
            // Search for a new target.
            for (int i = static_cast<int>(mouse_recievers.size()) - 1; i >= 0; i--) {
                if (gutils::contained_within({pos.x + cam.pos.x,pos.y + cam.pos.y}, mouse_recievers[i]->t)) {
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
