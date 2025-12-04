#pragma once
#include <iostream>

#include "engine/textures/FrameSheet.hpp"

namespace gengine::textures {
    // We create a registry of all the sprites that we need like this
    // (Make global vector)
    using SheetRegistry = std::unordered_map<int, FrameSheet>;

    // grabs the same registry each time
    inline SheetRegistry& getRegistry() {
        static SheetRegistry registry;
        return registry;
    }

    // This allows us to call function constructions even though we haven't entered main yet
    // super cool
    struct SheetRegister {
        // marked as explicit just cause
        explicit SheetRegister(int sheet_id, const FrameSheet& sheet) {
            getRegistry()[sheet_id] = sheet;
        }
    };
}
