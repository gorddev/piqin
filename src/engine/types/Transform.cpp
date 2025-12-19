#include "engine/types/Transform.hpp"

#include "engine/types/positioning/Point2D.hpp"


using namespace geng;

// Default constructor
Transform::Transform() = default;

// Explicit constructor for position
Transform::Transform(Vertex pos) : pos(pos) {}

// Defines position, height, and width
Transform::Transform(Vertex pos, uint16_t width, uint16_t height)
    : pos(pos), h(height), w(width), baseHeight(height), baseWidth(width) {}

// Defines position, height, width, and scale
Transform::Transform(Vertex pos, uint16_t width, uint16_t height, float scale)
    : pos(pos), h(height), w(width), baseHeight(height), baseWidth(width), scale(scale) {}

// Resetting width, height, and angle to base values
void Transform::reset() {
    w = baseWidth;
    h = baseHeight;
    angle = 0;
}

// Convert to string (debugging)
std::string Transform::to_string() const {
    std::string result = "{\n";

    result += "  scale: " + std::to_string(scale) + ",\n";
    result += "  angle: " + std::to_string(angle) + ",\n";
    result += "  width: " + std::to_string(w) + ",\n";
    result += "  height: " + std::to_string(h) + ",\n";
    result += "  baseWidth: " + std::to_string(baseWidth) + ",\n";
    result += "  baseHeight: " + std::to_string(baseHeight) + ",\n";

    result += "  color: {r:" + std::to_string(color.r) +
              ", g:" + std::to_string(color.g) +
              ", b:" + std::to_string(color.b) +
              ", a:" + std::to_string(color.a) + "},\n";
    result += "  pos: " + pos.to_string() + "\n";

    result += "}";
    return result;
}

// Get base dimensions
uint16_t Transform::get_base_width() const { return baseWidth; }
uint16_t Transform::get_base_height() const { return baseHeight; }

// Snap position to scene boundaries
void Transform::snap_to_scene(EngineContext& world) {
    if (pos.x < 0)
        pos.x = 0;
    else if (pos.x >= world.get_width())
        pos.x = world.get_width() - 1;
    if (pos.y < 0)
        pos.y = 0;
    else if (pos.y >= world.get_height())
        pos.y = world.get_height() - 1;
}

std::vector<SDL_FPoint> Transform::to_vertex_hitbox(uint16_t thickness) {
    // Convert to integer for nice rendering
    Point2D p = Point2D(pos.x, pos.y);
    // Get the REAL hitbox, not the displayed one in ints
    Dim2D d = {baseWidth/2, baseHeight/2};
    Box2D left = {p.x - d.w, p.y - d.h, thickness, d.h*2};
    Box2D top = {p.x - d.w , p.y - d.h, d.w*2, thickness};
    Box2D bottom = {p.x - d.w + thickness, p.y + d.h - thickness, d.w*2 - thickness, thickness};
    Box2D right = {p.x + d.w - thickness, p.y - d.h, thickness, d.h*2};

    std::vector<SDL_FPoint> points;
    left.to_vertex_points(points);
    top.to_vertex_points(points);
    bottom.to_vertex_points(points);
    right.to_vertex_points(points);
    return points;
}