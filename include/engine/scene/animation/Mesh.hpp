#pragma once
#include "Frame.hpp"
#include "../../types/positioning/Vertex.hpp"

namespace geng {
    /// incomplete type do not use.
    struct Mesh {
        Frame frame;
        Vertex pos;
        short texture_id;
    };
}
