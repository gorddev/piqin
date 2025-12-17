#include "engine/input/InputHandler.hpp"

#include "engine/scene/routes/Route.hpp"

using namespace geng;

void InputHandler::_key_press(SDL_Scancode key) {
    if (active) {
        heldKeys[key] = true;
        for (auto&g : key_acceptors) {
            g->on_key_press(key);
        }
    }
}

void InputHandler::_key_release(SDL_Scancode key) {
    if (active) {
        if (heldKeys.find(key) != heldKeys.end()) {
            heldKeys.erase(key);
        }
        for (auto&g : key_acceptors) {
            g->on_key_release(key);
        }
    }
}

void InputHandler::_mouse_click() {
    if (active)
        mouse.on_click();
}

void InputHandler::_mouse_release() {
    if (active)
        mouse.on_click_release();
}

void InputHandler::_mouse_move(SDL_Point pos, float dx, float dy) {
    if (active)
        mouse.on_movement(pos, dx, dy, *world);
}


// ************************************
//<><><> Adding/Removing Objects <><><>
// ************************************m
void InputHandler::add_key_acceptor(Gear *g) {
    key_acceptors.push_back(g);
}

void InputHandler::add_key_acceptors(std::vector<Gear *> &gears) {
    key_acceptors.insert(key_acceptors.end(), gears.begin(), gears.end());
}

void InputHandler::add_mouse_acceptor(Gear *g) {
    mouse_acceptors.push_back(g);
}

void InputHandler::add_mouse_acceptors(std::vector<Gear *> &gears) {
    mouse_acceptors.insert(mouse_acceptors.end(), gears.begin(), gears.end());
}

void InputHandler::remove_key_acceptor(Gear *g) {
    for (auto it = key_acceptors.begin(); it != key_acceptors.end(); it++) {
        if (*it == g) {
            key_acceptors.erase(it);
            break;
        }
    }
}

void InputHandler::remove_key_acceptors(std::vector<Gear *> &gears) {
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

void InputHandler::remove_mouse_acceptors(std::vector<Gear *> &gears) {
    for (auto& g: gears) {
        remove_mouse_acceptor(g);
    }
}

void InputHandler::flush() {
    key_acceptors.clear();
    mouse_acceptors.clear();
    mouse.pop_target();
}

void InputHandler::flush_keyboard_acceptors() {
    key_acceptors.clear();
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

void InputHandler::_add_engine_context(EngineContext *ctx) {
    world = ctx;
}

