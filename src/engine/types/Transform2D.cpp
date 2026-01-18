#include "engine/types/Transform2D.hpp"

#include "engine/types/positioning/Box2D.hpp"
#include <format>

#include "engine/types/positioning/FBox2D.hpp"


using namespace gan;

// Default constructor
Transform2D::Transform2D() = default;

// Explicit constructor for position
Transform2D::Transform2D(FPos2D pos) : pos(pos) {}

// Defines position, height, and width
Transform2D::Transform2D(FPos2D pos, uint16_t width, uint16_t height)
    : pos(pos), h(height), w(width), baseHeight(height), baseWidth(width) {}

// Defines position, height, width, and scale
Transform2D::Transform2D(FPos2D pos, uint16_t width, uint16_t height, float scale)
    : pos(pos), h(height), w(width), baseHeight(height), baseWidth(width), scale(scale) {}

// Resetting width, height, and angle to base values
void Transform2D::reset() {
    w = baseWidth;
    h = baseHeight;
    angle = 0;
}

// gets the adjusted position
FPos2D Transform2D::get_s_pos () {
    return { pos.x - get_base_width()/2.f, pos.y - get_base_height()/2.f };
}

void Transform2D::set_s_pos(FPos2D new_pos) {
    pos = {new_pos.x + get_base_width()/2.f, new_pos.y + get_base_height()/2.f};
}

// Get base dimensions
uint16_t Transform2D::get_base_width() const { return baseWidth; }
uint16_t Transform2D::get_base_height() const { return baseHeight; }

// Snap position to scene boundaries
void Transform2D::snap_to_scene(LayerContext& scene) {
    if (pos.x < scene.get_camera().pos.x)
        pos.x = scene.get_camera().pos.x;
    else if (pos.x >= scene.get_camera().right())
        pos.x = scene.get_camera().right()-1;
    if (pos.y < 0)
        pos.y = 0;
    else if (pos.y >= scene.get_camera().bottom())
        pos.y = scene.get_camera().bottom() - 1;
}

gch::vector<SDL_FPoint> Transform2D::to_vertex_hitbox(uint16_t thickness) {
    // Convert to integer for nice rendering
    FPos2D p = FPos2D(pos.x, pos.y);
    // Get the REAL hitbox, not the displayed one in ints
    FPos2D d = {baseWidth*scale/2, baseHeight*scale/2};
    FBox2D left = {p.x - d.x, p.y - d.y, thickness, static_cast<int>(d.y*2)};
    FBox2D top = {p.x - d.x , p.y - d.y, static_cast<int>(d.x*2), thickness};
    FBox2D bottom = {p.x - d.x + thickness, p.y + d.y - thickness, static_cast<int>(d.x*2 - thickness), thickness};
    FBox2D right = {p.x + d.x - thickness, p.y - d.y, thickness, static_cast<int>(d.y*2)};

    gch::vector<SDL_FPoint> points;
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


