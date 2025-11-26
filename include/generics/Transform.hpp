#pragma once
#include "Vertex.hpp"

struct Transform {
    // Size of each side of the object
    double height = 10;
    double width = 10;
    // Scale of the object
    float scale = 1;
    // Rotation of the object
    float rotation = 0;
    // Zin-dex
    float z_index = 0;
    // Establish Vertexes for pixel-perfect rendering
    Vertex pos = Vertex(0,0,0);

    Transform() = default;
    Transform(Vertex pos) : pos(pos) {}
    Transform(Vertex pos, double height, double width) : pos(pos), height(height), width(width) {}
    Transform(Vertex pos, double height, double width, double scale) : pos(pos), height(height), width(width), scale(scale) {}
    Transform(Vertex pos, double height, double width, float scale, float z_index) : pos(pos), height(height), width(width), scale(scale), z_index(z_index) {}
};