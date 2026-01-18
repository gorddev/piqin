#include "engine/input/InputDistributor.hpp"

#include <SDL.h>
#include <utility>

#include "engine/debug/geng_debug.hpp"
#include "engine/utilities/Utilities.hpp"

#define keyb key.keysym.scancode

using namespace gan;


uint8_t *& InputDistributor::get_keystates() {
    return key_states;
}

InputDistributor::InputDistributor(EngineContext &e) : world(e), key_states(nullptr) {}

void InputDistributor::event_key_down(SDL_Scancode key, Layer*& active_layer) {
    bool consumed = false;
    for (auto& r : routers) {

        if (!consumed)
            consumed = r->get_key_press(key, active_layer);
        else
            break;
    }
    if (active_layer != nullptr && !consumed) {
        active_layer->input._get_key_press(key);
    }
}

void InputDistributor::event_key_up(SDL_Scancode key, Layer*& active_layer) {
    bool consumed = false;
    for (auto& r : routers) {
        if (!consumed)
            consumed = r->get_key_release(key, active_layer);
        else
            break;
    }
    if (active_layer != nullptr && !consumed) {
        active_layer->input._get_key_release(key);
    }
}

void InputDistributor::event_mouse_click(const SDL_Event& e, Layer*& active_layer) {
    Pos2D mousepos;
    // Gets the mouse state
    SDL_GetMouseState(&mousepos.x, &mousepos.y);
    // Adjust coordinates to relative coordinates
    gutils::adjust_to_relative_coords(mousepos, world);
    bool consumed = false;
    for (auto& r : routers) {
        if (!consumed)
            consumed = r->get_mouse_click(e.button.button, mousepos, active_layer);
        else
            break;
    }
    if (active_layer != nullptr && !consumed) {
        active_layer->input._get_mouse_click(mousepos);
    }
}

void InputDistributor::event_mouse_release(const SDL_Event& e, Layer*& active_layer) {
    Pos2D mousepos;
    // Gets the mouse state
    SDL_GetMouseState(&mousepos.x, &mousepos.y);
    // Adjust coordinates to relative coordinates
    gutils::adjust_to_relative_coords(mousepos, world);
    bool consumed = false;
    for (auto& r : routers) {
        if (!consumed)
            consumed = r->get_mouse_release(e.button.button, mousepos, active_layer);
        else
            break;
    }
    if (active_layer != nullptr && !consumed) {
        active_layer->input._get_mouse_release(mousepos);
    }
}

void InputDistributor::event_mouse_motion(const SDL_Event& e, Layer*& active_layer) {
    Pos2D mousepos;
    // Gets the mouse state
    SDL_GetMouseState(&mousepos.x, &mousepos.y);
    // Find change in position
    FPos2D deltapos = {e.motion.xrel/world.get_scale(),
        e.motion.yrel/world.get_scale()};
    // Adjust coordinates to relative coordinates
    gutils::adjust_to_relative_coords(mousepos, world);
    bool consumed = false;
    for (auto& r : routers) {
        if (!consumed)
            consumed = r->get_mouse_move(mousepos, deltapos, active_layer);
        else
            break;
    }
    if (active_layer != nullptr && !consumed) {
        active_layer->input._mouse_move(mousepos, deltapos);
    }
}

void InputDistributor::event_finger_up(SDL_TouchFingerEvent finger, Layer*& active_layer) {
    bool consumed = false;
    for (auto& r : routers) {
        if (!consumed)
            consumed = r->get_finger_release(finger, active_layer);
        else
            break;
    }
    if (active_layer != nullptr && !consumed) {
        active_layer->input._finger_release(finger);
    }
}

void InputDistributor::event_finger_down(SDL_TouchFingerEvent finger, Layer*& active_layer) {
    bool consumed = false;
    glog::note << "finger down?" << glog::endlog;
    for (auto& r : routers) {
        if (!consumed)
            consumed = r->get_finger_down(finger, active_layer);
        else
            break;
    }
    if (active_layer != nullptr && !consumed) {
        active_layer->input._finger_down(finger);
    }
}

void InputDistributor::event_finger_motion(SDL_TouchFingerEvent finger, Layer*& active_layer) {
    bool consumed = false;
    for (auto& r : routers) {
        if (!consumed)
            consumed = r->get_finger_motion(finger, active_layer);
        else
            break;
    }
}

void InputDistributor::event_mouse_wheel(SDL_MouseWheelEvent e, Layer*& active_layer) {
    bool consumed = false;
    for (auto& r : routers) {
        if (!consumed) {
            consumed = r->get_mouse_scroll(e, active_layer);
        }
        else
            break;
    }
}

void InputDistributor::process_event(const SDL_Event& e, Layer* active_layer) {
    if (e.type == SDL_KEYDOWN)
        event_key_down(e.key.keysym.scancode, active_layer);
    else if (e.type == SDL_KEYUP)
        event_key_up(e.key.keysym.scancode, active_layer);
    else if (e.type == SDL_MOUSEBUTTONDOWN)
        event_mouse_click(e, active_layer);
    else if (e.type == SDL_MOUSEBUTTONUP)
        event_mouse_release(e, active_layer);
    else if (e.type == SDL_MOUSEMOTION)
        event_mouse_motion(e, active_layer);
    else if (e.type == SDL_FINGERUP)
        event_finger_up(e.tfinger, active_layer);
    else if (e.type == SDL_FINGERDOWN)
        event_finger_down(e.tfinger, active_layer);
    else if (e.type == SDL_FINGERMOTION)
        event_finger_motion(e.tfinger, active_layer);
    else if (e.type == SDL_MOUSEWHEEL) {
        event_mouse_wheel(e.wheel, active_layer);
    }

}

void InputDistributor::update(Layer* active_layer) {
    const uint8_t* keys = static_cast<const uint8_t*>(SDL_GetKeyboardState(nullptr));
    bool consumed = false;
    for (auto& r : routers) {
        if (r->update(active_layer, keys))
            break;
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

void InputDistributor::emplace_key_states(Layer* layer) const {
    layer->input._keys_down(key_states);
}

void InputDistributor::_init() {
    key_states = const_cast<uint8_t*>(SDL_GetKeyboardState(NULL));
}
