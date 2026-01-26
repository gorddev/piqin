#pragma once
#include "default-debug-syntax-map.hpp"
#include "engine/mods/banners/text/Text.hpp"

namespace gan::debug {

    /** Text used for debugging, which has a black background */
    class DebugText : public Text {
    private:

    public:
        DebugText(const char text[], Font& font)
            : Text(text, font, Align::LEFT, &gan_default_debug_syntax_map) {}

    };

}
