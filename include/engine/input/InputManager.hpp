#pragma once
#include "InputQueue.hpp"
#include "InputTarget.hpp"
#include <SDL.h>
#include <unordered_map>

namespace gengine {
    class InputManager {
        InputQueue iqueue;
        InputTarget* it;
        InputTarget* menu;
        std::unordered_map<SDL_Scancode, GENG_Input> mapToBindings;
        std::unordered_map<GENG_Input, bool> pressedKeys;
        std::unordered_map<GENG_Input, bool> nullMap;

        // Gets the keybind for each scancode
        GENG_Input keybind(SDL_Scancode key);
    public:
        // Constructor, we need the menu
        explicit InputManager(InputTarget* menu);
        // Update with a given key event
        void update(SDL_Event & e);
        // Initializes the keybinds
        void load_keybinds(std::unordered_map<SDL_Scancode, short> mappy);
        // If we're in a menu or not
        bool in_menu() const { return it == menu; }
        // Sets the input target
        void setInputTarget(InputTarget* target);
        // Gets if a key is pressed down.
    };
}