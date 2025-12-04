#pragma once
#include "Frame.hpp"
#include "engine/utilities/types/Vertex.hpp"

namespace gengine {
    struct Mesh {
        textures::Frame frame;
        Vertex pos;
        short texture_id;
    };
}
