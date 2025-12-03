#pragma once
#include <iostream>

#include "../SpriteSheet.hpp"

namespace gengine {
    // We create a registry of all the sprites that we need like this
    // (Make global vector)
    using SpriteRegistry = std::unordered_map<int, SpriteSheet>;

    // grabs the same registry each time
    inline SpriteRegistry& getRegistry() {
        static SpriteRegistry registry;
        return registry;
    }

    // This allows us to call function constructions even though we haven't entered main yet
    // super cool
    struct SpriteRegister {
        // marked as explicit just cause
        explicit SpriteRegister(const SpriteSheet& sheet, int sheet_id) {
            getRegistry()[sheet_id] = sheet;
        }
    };
}