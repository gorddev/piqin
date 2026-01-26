#pragma once
#include "../core/gears/Gear.hpp"
#include "engine/utilities/Utilities.hpp"
#include "engine/rendering/Camera.hpp"
#include "engine/layers/layer-subobjects/LayerCore.hpp"

namespace gan {

    /** @brief Holds basic information for a mouse to be used by the Inputhandler*
     * @details Mouse has the following member variables
     */
    struct Mouse {

    public:
        /// Coordinates of the mouse
        pos2 pos;
        /// Change in position of mouse frame to frame
        vec2 dpos;
        /// Attatchment point of object relative to center of mouse
        vec2 relpos = {0, 0};
        /// Target of mouse action
        Gear* target = nullptr;
        /// Whether the mouse is down or not.
        bool down = false;
        /// Reference to the vector of mouse recievers.
        std::vector<Gear*>& mouse_recievers;
        /// Gets access to the camera
        const Camera& cam;
        /// Gets access to the current scene state
        LayerState& state;

        /// This constructor links the mouse to the vector it oversees.
        explicit Mouse(std::vector<Gear*>& mouse_recievers, const Camera& cam, LayerState& state)
            : mouse_recievers(mouse_recievers), cam(cam), state(state) {}

        /// Does things to a target if the mouse is hovering over something right now.
        void on_click() {
            down = true;

            if (target != nullptr) {
                auto mpos = mouse_world();
                target->_engine_flagger(GFlag::clicked);
                target->on_click(pos);
                if (target->is_draggable()) {
                    relpos.x = mpos.x - target->t.pos.x;
                    relpos.y = mpos.y - target->t.pos.y;
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
                target->on_click_release(pos);
            }
        }

        /// Pops the current target for if the target becomes invalid
        void pop_target() {
            if (target != nullptr) {
                flush_target();
            }
        }

        vec2 mouse_world() {
            vec2 w;

            w.x = pos.x *cam.get_width()/ state.get_canvas_width();
            w.y = pos.y*cam.get_height()/state.get_canvas_height();

            w.x += cam.pos.x - cam.offset.x;
            w.y += cam.pos.y - cam.offset.y;

            return w;
        }

        bool contained_within(const Transform2D& t) {
            SDL_FPoint m = mouse_world();

            return
                m.x >= t.pos.x - t.w*t.scale/2 &&
                m.x < t.pos.x + t.w*t.scale/2 &&
                m.y >= t.pos.y - t.h*t.scale/2 &&
                m.y < t.pos.y + t.h*t.scale/2;
        }

        /// Called when the mouse is moved by some distance.
        void on_movement(pos2 mousepos, vec2 deltapos, LayerCore& scene) {
            // First we need to sort by z-index first.
            std::sort(mouse_recievers.begin(), mouse_recievers.end(), [](const Gear* a, const Gear* b) {
                return a->z_index > b->z_index;
            });
            // First update our cursor position
            pos = {mousepos.x, mousepos.y};
            dpos = {deltapos.x, deltapos.x};

            // If we currently have a target
            if (target != nullptr) {
                // If we're dragging, we update position.
                if (target->is_dragged()) {
                    auto mpos = mouse_world();
                    target->on_drag({static_cast<int>(mpos.x - relpos.x),static_cast<int>(mpos.y - relpos.y)});
                    return;
                }
                if (!contained_within(target->t)) {
                    target->on_hover_release();
                    target->_engine_deflagger(GFlag::hovered);
                    target = nullptr;
                }
            }
            auto oldtarget = target;
            target = nullptr;
            // Search for a new target.
            for (int i = static_cast<int>(mouse_recievers.size()) - 1; i >= 0; i--) {
                if (contained_within(mouse_recievers[i]->t)) {
                    target = mouse_recievers[i];
                    if (target != oldtarget) {
                        target->on_hover();
                        target->_engine_flagger(GFlag::hovered);
                        if (oldtarget != nullptr) {
                            oldtarget->on_hover_release();
                            target->_engine_deflagger(GFlag::hovered);
                        }
                    }
                    break;
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
