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
    InputManager(InputTarget* menu);

    void update(SDL_Event & e);

    void load_keybinds(std::unordered_map<SDL_Keycode, short> mappy);

    bool in_menu() const { return it == menu; }

    void setInputTarget(InputTarget* target);
};