#pragma once
#include <unordered_map>

class InputTarget {
public:
    // All the keys that are currently held down
    std::unordered_map<short, bool>* heldKeys;
    // The current key that's pressed down
    short keyPress;
    // Default constructor
    InputTarget() = default;
    // Gets a held key
    void getHeld(short keybind) {(*heldKeys)[keybind] = true;};

    // Gets if a key is held
    bool is_held(short keybind) {
        if (heldKeys->find(keybind) != heldKeys->end())
            return true;
        return false;
    }
    // Gets a pressed key. Must be virtual cause every interface will have valid/invalid inputs
    // Also must return a boolean.
    virtual bool getPress(short keybind) = 0;
    // Gets the release of a held key
    virtual void getRelease(short keybind) = 0;

    virtual ~InputTarget() = default;
};
