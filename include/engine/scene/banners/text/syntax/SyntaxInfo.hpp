#pragma once
#include <SDL_pixels.h>


namespace gan {
    /// The object used to keep track of parsing information as you go through the parse.
    struct SyntaxInfo {
        SDL_Color color = {255, 255, 255, 255};
        float scale = 1.f;
        bool italics = false;
        /// Default constructor
        SyntaxInfo() = default;
    };
}
