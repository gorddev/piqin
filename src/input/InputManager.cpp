#include "input/InputManager.hpp"

#include <iostream>
#include <SDL.h>

#define keyb key.keysym.scancode

InputManager::InputManager(InputTarget* menu) : menu(menu) {
    load_keybinds({{SDL_SCANCODE_0, 0}});
}

void InputManager::load_keybinds(std::unordered_map<SDL_Scancode, short> mappy) {
    mapToBindings = mappy;
    mapToBindings[SDL_SCANCODE_LSHIFT] = INPUT_MODIFY;
    mapToBindings[SDL_SCANCODE_W] = INPUT_UP;
    mapToBindings[SDL_SCANCODE_S] = INPUT_DOWN;
    mapToBindings[SDL_SCANCODE_D] = INPUT_RIGHT;
    mapToBindings[SDL_SCANCODE_A] = INPUT_LEFT;
    mapToBindings[SDL_SCANCODE_J] = INPUT_SELECT;
    mapToBindings[SDL_SCANCODE_K] = INPUT_BACK;
    mapToBindings[SDL_SCANCODE_SPACE] = INPUT_SELECT_ALT;
    mapToBindings[SDL_SCANCODE_P] = INPUT_PAUSE;
}

void InputManager::setInputTarget(InputTarget *target) {
    if (it != nullptr)
        it->heldKeys = &nullMap;
    it = target;
    it->heldKeys = &pressedKeys;
}

short InputManager::keybind(SDL_Scancode key) {
    if (mapToBindings.find(key) != mapToBindings.end())
        return mapToBindings[key];
    return INPUT_NULL;
}

void InputManager::update(SDL_Event& e) {

    const Uint8* keyHeld = SDL_GetKeyboardState(0);
    // Update our queue and discard
    iqueue.update();
    if (e.type == SDL_KEYDOWN && e.key.repeat == false) {
        // Swapping to and from the menu.
        if (INPUT_PAUSE == keybind(e.keyb) && !scene::mainManu) {
            // If we're not in the menu enter the menu
            if (it != menu && menu != nullptr) {
                targetStack.push(it);
                it = menu;
            }
            // Otherwise we return to the previous input.
            else if (menu != nullptr){
                it = targetStack.top();
                targetStack.pop();
            }
        }
        iqueue.push(keybind(e.keyb)); // NOLINT(*-narrowing-conversions)
        it->getPress(keybind(e.keyb));
    }
    // Holding a key down, update shit
    if (e.type == SDL_KEYDOWN)
        pressedKeys[keybind(e.keyb)] = true;
    // Letting a key up, update shit
    else if (e.type == SDL_KEYUP) {
        pressedKeys[keybind(e.keyb)] = false;
        if (it != nullptr) {
            it->getRelease(keybind(e.keyb));
        }
    }
}
