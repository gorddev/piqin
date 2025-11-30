#pragma once
#include <unordered_map>

namespace gengine {
    class InputTarget {
    public:
        // All the keys that are currently held down
        std::unordered_map<GENG_Input, bool>* heldKeys;
        // Default constructor
        InputTarget() = default;
        // Gets a held key
        void getHeld(GENG_Input keybind) {(*heldKeys)[keybind] = true;};

        // Gets if a key is held
        bool is_held(GENG_Input keybind) {
            if (heldKeys->find(keybind) != heldKeys->end())
                return true;
            return false;
        }
        // Gets a pressed key. Must be virtual cause every interface will have valid/invalid inputs
        // Also must return a boolean.
        virtual bool getPress(GENG_Input keybind) = 0;
        // Gets the release of a held key
        virtual void getRelease(GENG_Input keybind) = 0;

        virtual ~InputTarget() = default;
    };
}