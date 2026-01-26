#include "engine/types/Transform2D.hpp"

#include "engine/types/positioning/box2.hpp"
#include <format>

#include "engine/types/positioning/fbox2.hpp"


using namespace gan;

// Default constructor
Transform2D::Transform2D() = default;

// Explicit constructor for position
Transform2D::Transform2D(vec2 pos) : pos(pos) {}

// Defines position, height, and width
Transform2D::Transform2D(vec2 pos, uint16_t width, uint16_t height)
    : pos(pos), h(height), w(width), baseHeight(height), baseWidth(width) {}

// Defines position, height, width, and scale
Transform2D::Transform2D(vec2 pos, uint16_t width, uint16_t height, float scale)
    : pos(pos), h(height), w(width), baseHeight(height), baseWidth(width), scale(scale) {}

// Resetting width, height, and angle to base values
void Transform2D::reset() {
    w = baseWidth;
    h = baseHeight;
    angle = 0;
}

// gets the adjusted position
vec2 Transform2D::get_s_pos () {
    return { pos.x - get_base_width()/2.f, pos.y - get_base_height()/2.f };
}

void Transform2D::set_s_pos(vec2 new_pos) {
    pos = {new_pos.x + get_base_width()/2.f, new_pos.y + get_base_height()/2.f};
}

// Get base dimensions
uint16_t Transform2D::get_base_width() const { return baseWidth; }
uint16_t Transform2D::get_base_height() const { return baseHeight; }

std::vector<SDL_FPoint> Transform2D::to_vertex_hitbox(uint16_t thickness) {
    // Convert to integer for nice rendering
    vec2 p = vec2(pos.x, pos.y);
    // Get the REAL hitbox, not the displayed one in ints
    vec2 d = {baseWidth*scale/2, baseHeight*scale/2};
    fbox2 left = {p.x - d.x, p.y - d.y, thickness, static_cast<int>(d.y*2)};
    fbox2 top = {p.x - d.x , p.y - d.y, static_cast<int>(d.x*2), thickness};
    fbox2 bottom = {p.x - d.x + thickness, p.y + d.y - thickness, static_cast<int>(d.x*2 - thickness), thickness};
    fbox2 right = {p.x + d.x - thickness, p.y - d.y, thickness, static_cast<int>(d.y*2)};

    std::vector<SDL_FPoint> points;
    left.to_vert_points(points);
    top.to_vert_points(points);
    bottom.to_vert_points(points);
    right.to_vert_points(points);
    return points;
}

str_view& Transform2D::to_fstring_verbose(str_view& buffer) const {
    buffer << precision<2>() << "[t][Transform][n] \tscl: " << scale << "\tang: " << angle
        << "\tcolor: {" << color.r + 0 <<   ",\t" << color.g << ",\t" << color.b << "}\n"
    << "dim: {" << w << ",  \t" << h << "}\tbase: [" << baseWidth << ", " << baseHeight << "]\n"
    << "pos: {" << pos.x << ",  \t" << pos.y << "}\toffset: {" << offset.x << ",\t" << offset.y << "}";
    return buffer;
}

str_view& Transform2D::to_fstring(str_view& buffer) const {
    buffer << precision<2>() << "[[t]Tsfm[n]]\t" << "pos: {" << pos.x << ",  \t" << pos.y << "} \tdim: {" << w << ",  \t" << h << "}";
    return buffer;
}


