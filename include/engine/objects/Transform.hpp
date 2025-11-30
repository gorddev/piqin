#pragma once
#include "../utilities/types/Vertex.hpp"

namespace gengine {
    struct Transform {
        // Size of each side of the object
        short height = 10;
        short width = 10;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        // Scale of the object
        float scale = 1;
        // Rotation of the object
        float rotation = 0;
        // Establish Vertexes for pixel-perfect rendering & z-indexing
        Vertex pos = Vertex(0,0,0);
        // Establish offset for rendering purposes
        Vertex offset = Vertex(0,0,0);

        Transform() = default;
        explicit Transform(Vertex pos) : pos(pos) {}
        Transform(Vertex pos, float height, float width) : pos(pos), height(height), width(width) {}
        Transform(Vertex pos, float height, float width, float scale) : pos(pos), height(height), width(width), scale(scale) {}
    };
}