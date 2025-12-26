#include "engine/input/InputHandler.hpp"

#include "engine/scene/routes/Route.hpp"

using namespace geng;

InputHandler::InputHandler(LayerContext& scene) : scene(scene), cam(scene.get_camera()), mouse(mouse_acceptors, scene.get_camera()) {}

void InputHandler::_get_key_press(SDL_Scancode key) {
    if (active) {
        heldKeys[key] = true;
        for (auto&g : key_press_acceptors) {
            g->on_key_press(key);
        }
    }
}

void InputHandler::_get_key_release(SDL_Scancode key) {
    if (active) {
        if (heldKeys.find(key) != heldKeys.end()) {
            heldKeys.erase(key);
        }
        for (auto&g : key_press_acceptors) {
            g->on_key_release(key);
        }
    }
}

void InputHandler::_get_mouse_click(Pos2D mousepos) {
    mouse.on_click();
}

void InputHandler::_get_mouse_release(Pos2D mousepos) {
    mouse.on_click_release();
}


void InputHandler::_mouse_move(Pos2D mousepos, FPos2D deltapos) {
    mouse.on_movement(mousepos, deltapos, scene);
}

void InputHandler::_keys_down(uint8_t *keys) const {
    scene.state._set_held_keys(keys);
}


// ************************************
//<><><> Adding/Removing Objects <><><>
// ************************************m
void InputHandler::add_key_press_acceptor(Gear *g) {
    key_press_acceptors.push_back(g);
}

void InputHandler::add_key_press_acceptors(gch::vector<Gear *> &gears) {
    key_press_acceptors.insert(key_press_acceptors.end(), gears.begin(), gears.end());
}

void InputHandler::add_mouse_acceptor(Gear *g) {
    mouse_acceptors.push_back(g);
}

void InputHandler::add_mouse_acceptors(gch::vector<Gear *> &gears) {
    mouse_acceptors.insert(mouse_acceptors.end(), gears.begin(), gears.end());
}

void InputHandler::remove_key_acceptor(Gear *g) {
    for (auto it = key_press_acceptors.begin(); it != key_press_acceptors.end(); it++) {
        if (*it == g) {
            key_press_acceptors.erase(it);
            break;
        }
    }
}

void InputHandler::remove_key_acceptors(gch::vector<Gear *> &gears) {
    for (auto& g: gears) {
        remove_key_acceptor(g);
    }
}
void InputHandler::remove_mouse_acceptor(Gear *g) {
    for (auto it = mouse_acceptors.begin(); it != mouse_acceptors.end(); it++) {
        if (*it == g) {
            if (g == mouse.target)
                mouse.pop_target();
            mouse_acceptors.erase(it);
            break;
        }
    }
}

void InputHandler::remove_mouse_acceptors(gch::vector<Gear *> &gears) {
    for (auto& g: gears) {
        remove_mouse_acceptor(g);
    }
}

void InputHandler::flush() {
    key_press_acceptors.clear();
    mouse_acceptors.clear();
    mouse.pop_target();
}

void InputHandler::flush_keyboard_acceptors() {
    key_press_acceptors.clear();
}

void InputHandler::flush_mouse_acceptors() {
    mouse_acceptors.clear();
    mouse.pop_target();
}

void InputHandler::refresh() {
    mouse.flush_target();
}

void InputHandler::pause() {
    mouse.flush_target();
    active = false;
}

void InputHandler::resume() {
    active = true;
}

