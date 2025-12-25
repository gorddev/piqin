#include "engine/input/InputDistributor.hpp"

#include <SDL.h>
#include <utility>

#include "../../../include/engine/types/EngineEnums.hpp"
#include "engine/utilities/Utilities.hpp"
#include "engine/scene/sprites/Sprite.hpp"

#define keyb key.keysym.scancode

using namespace geng;


uint8_t *& InputDistributor::get_keystates() {
    return key_states;
}

InputDistributor::InputDistributor(EngineContext &e) : world(e), key_states(nullptr) {}

void InputDistributor::process_event(const SDL_Event& e, Layer* active_layer) {
    bool consumed = false;
    for (auto& r : routers) {
        if (!consumed) {
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
        }
        else
            break;
    }
    if (active_layer != nullptr && !consumed) {
        active_layer->input._keys_down(key_states);
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
    else if (active_layer != nullptr)
        active_layer->input._keys_down(nullptr);
}

void InputDistributor::update(Layer* active_layer) {
    bool consumed = false;
    for (auto& r : routers) {
        if (!consumed) {
            r->_update_key_pointer(key_states);
            consumed = r->update(active_layer);
        }
        else
            r->_update_key_pointer(nullptr);
    }
}

void InputDistributor::add_input_router(InputRouter *router) {
    routers.push_back(router);
}

void InputDistributor::remove_input_router(InputRouter *router) {
    for (auto it = routers.begin(); it != routers.end(); ++it) {
        if ((*it) == router) {
            routers.erase(it);
            return;
        }
    }
}

void InputDistributor::_init() {
    key_states = const_cast<uint8_t*>(SDL_GetKeyboardState(NULL));
}
