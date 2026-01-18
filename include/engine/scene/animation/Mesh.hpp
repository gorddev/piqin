#pragma once
#include "Frame.hpp"
#include "../../types/positioning/FPos2D.hpp"

namespace gan {
    /// incomplete type do not use.
    struct Mesh {
        Frame frame;
        FPos2D pos;
        short texture_id;
    };
}
