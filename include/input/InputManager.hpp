#pragma once
#include "InputQueue.hpp"
#include "InputTarget.hpp"
#include <SDL.h>
#include <unordered_map>
#include <stack>

class InputManager {
    InputQueue iqueue;
    InputTarget* it;
    InputTarget* menu;
    std::stack<InputTarget*> targetStack;
    std::unordered_map<SDL_Keycode, short> mapToBindings;
    std::unordered_map<short, bool> pressedKeys;

    // Gets the keybind for each scancode
    int keybind(SDL_Keycode key);
public:
    // Constructor, we need the menu
    explicit InputManager(InputTarget* menu);
    // Update with a given key event
    void update(SDL_Event & e);
    // Initializes the keybinds
    void load_keybinds(std::unordered_map<SDL_Keycode, short> mappy);
    // If we're in a menu or not
    bool in_menu() const { return it == menu; }
    // Sets the input target
    void setInputTarget(InputTarget* target);
    // Gets if a key is pressed down.
};