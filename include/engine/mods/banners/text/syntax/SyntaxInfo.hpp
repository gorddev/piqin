#pragma once
#include <SDL_pixels.h>


namespace gan {
    /// The object used to keep track of parsing information as you go through the parse.
    struct SyntaxInfo {
        SDL_Color color = {gan::max_alpha, gan::max_alpha, gan::max_alpha, gan::max_alpha};
        int scale = 1;
        bool italics = false;
        /// Default constructor
        SyntaxInfo() = default;
    };
}
