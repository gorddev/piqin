#include "engine/input/InputSource.hpp"

#include <SDL.h>
#include <utility>

#include "../../../include/engine/types/EngineEnums.hpp"
#include "engine/utilities/Utilities.hpp"
#include "engine/scene/actors/Actor.hpp"

#define keyb key.keysym.scancode

using namespace geng;


InputSource::InputSource(EngineContext &e) : world(e) {}

void InputSource::update(const SDL_Event& e, Layer* active_layer) const {
    for (auto& r : routers) {
        bool consumed = false;
        if (e.type == SDL_KEYDOWN)
            consumed = r->get_press(e.key.keysym.scancode, active_layer);
        else if (e.type == SDL_KEYUP)
            consumed = r->get_release(e.key.keysym.scancode, active_layer);
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            consumed = r->get_click(e.button.button, x, y, active_layer);
        }
        else if (e.type == SDL_MOUSEBUTTONUP) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            consumed = r->get_click_release(e.button.button, x, y, active_layer);
        }
        else if (e.type == SDL_MOUSEMOTION) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            consumed = r->get_mouse_move(x, y, active_layer);
        }
        if (consumed)
            return;
    }
    if (active_layer != nullptr) {
        if (e.type == SDL_KEYDOWN && e.key.repeat == false) {
            // Sends the press to the input taret
            active_layer->input._key_press(e.key.keysym.scancode);
        }
        // Letting a key up, update shit
        else if (e.type == SDL_KEYUP) {
            active_layer->input._key_release(e.key.keysym.scancode);
        }
        // If it's a mouse button instead
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            active_layer->input._mouse_click();
        }
        else if (e.type == SDL_MOUSEBUTTONUP) {
            active_layer->input._mouse_release();
        }
        else if (e.type == SDL_MOUSEMOTION) {
            SDL_Point pos;
            SDL_GetMouseState(&pos.x, &pos.y);
            gutils::adjust_to_relative_coords(pos, world);
            float dx = e.motion.xrel/world.get_scale();
            float dy = e.motion.yrel/world.get_scale();
            active_layer->input._mouse_move(pos, dx, dy);
        }
    }
}

void InputSource::add_input_router(InputRouter *router) {
    routers.push_back(router);
}

void InputSource::remove_input_router(InputRouter *router) {
    for (auto it = routers.begin(); it != routers.end(); ++it) {
        if ((*it) == router) {
            routers.erase(it);
            return;
        }
    }
}