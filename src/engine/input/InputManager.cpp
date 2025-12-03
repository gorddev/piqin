#include "engine/input/InputManager.hpp"

#include <SDL.h>

#define keyb key.keysym.scancode

using namespace gengine;

InputManager::InputManager(InputTarget* menu) : menu(menu) {
    load_keybinds({{SDL_SCANCODE_0, GENG_Input::NONE}});
}

void InputManager::load_keybinds(std::unordered_map<SDL_Scancode, GENG_Input> mappy) {
    mapToBindings = mappy;
    mapToBindings[SDL_SCANCODE_LSHIFT] = GENG_Input::MODIFY;
    mapToBindings[SDL_SCANCODE_W] = GENG_Input::UP;
    mapToBindings[SDL_SCANCODE_S] = GENG_Input::DOWN;
    mapToBindings[SDL_SCANCODE_D] = GENG_Input::RIGHT;
    mapToBindings[SDL_SCANCODE_A] = GENG_Input::LEFT;
    mapToBindings[SDL_SCANCODE_J] = GENG_Input::SELECT;
    mapToBindings[SDL_SCANCODE_K] = GENG_Input::BACK;
    mapToBindings[SDL_SCANCODE_SPACE] = GENG_Input::SELECT_ALT;
    mapToBindings[SDL_SCANCODE_P] = GENG_Input::PAUSE;
}

void InputManager::setInputTarget(InputTarget *target) {
    if (it != nullptr)
        it->heldKeys = &nullMap;
    it = target;
    it->heldKeys = &pressedKeys;
}

GENG_Input InputManager::keybind(SDL_Scancode key) {
    if (mapToBindings.find(key) != mapToBindings.end())
        return mapToBindings[key];
    return GENG_Input::NONE;
}

void InputManager::update(SDL_Event& e) {

    const Uint8* keyHeld = SDL_GetKeyboardState(0);
    // Update our queue and discard
    iqueue.update();
    if (e.type == SDL_KEYDOWN && e.key.repeat == false) {
        // Swapping to and from the menu.

        iqueue.push(keybind(e.keyb)); // NOLINT(*-narrowing-conversions)
        it->get_press(keybind(e.keyb));
    }
    // Holding a key down, update shit
    if (e.type == SDL_KEYDOWN)
        pressedKeys[keybind(e.keyb)] = true;
    // Letting a key up, update shit
    else if (e.type == SDL_KEYUP) {
        pressedKeys[keybind(e.keyb)] = false;
        if (it != nullptr) {
            it->get_release(keybind(e.keyb));
        }
    }
}
