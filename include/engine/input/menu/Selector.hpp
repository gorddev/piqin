#pragma once
#include "engine/objects/Object.hpp"

namespace gengine {
    // Generic selector object
    template <typename T>
    struct Selector : Object {
        // Target of the selector
        T* target;

        // Constructor
        Selector(): target(nullptr) {}

        // Then we implement our selector movements
        virtual void move() {move(false); };
        virtual void move(bool recess) = 0;
        virtual void move(Vertex pos, float speed) = 0;

        // Make sure that we can define switching of targetgs
        virtual void switchTarget(T* newTarget, bool moveTarget) = 0;

        // Lets us deactivate the selector if needed
        virtual void deactivate() = 0;

        // Lets us shake the current selection if we try an move out of bounds;
        virtual void shake_target() = 0;;
    };
}
