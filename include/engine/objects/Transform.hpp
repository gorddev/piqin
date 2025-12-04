#pragma once
#include "../utilities/types/Vertex.hpp"

namespace gengine {
    struct Transform {
        // Scale of the object
        float scale = 1;
        // Rotation of the object
        float angle = 0;
        // Size of each side of the object
        short h = 10;
        short w = 10;
        // If we're flipped or not
        bool flipX = false;
        bool flipY = false;
        bool unlock = false;
        // Color of the object
        SDL_Color color = {255, 255, 255, 255};

        // Establish Vertexes for pixel-perfect rendering & z-indexing
        Vertex pos = Vertex(0,0,0);
        // Establish offset for rendering purposes
        Vertex offset = Vertex(0,0,0);

        Transform() = default;
        explicit Transform(Vertex pos) : pos(pos) {}
        Transform(Vertex pos, float height, float width) : pos(pos), h(height), w(width) {}
        Transform(Vertex pos, float height, float width, float scale) : pos(pos), h(height), w(width), scale(scale) {}
    };
}