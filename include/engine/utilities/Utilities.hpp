#pragma once

#include "../core/EngineContext.hpp"
#include "engine/types/Transform2D.hpp"
#include "engine/types/positioning/box2.hpp"

namespace gutils {

    constexpr float pi = 3.14159265358;
    constexpr float degreesToRadians = (pi / 180.0f);
    constexpr float roottwo = 1.4142135624;

    // Returns the sign of a variable to 4 digits (-1, 0, 1)
    signed char sgn(long double);

    /// Returns true if the point is within the transform.
    bool contained_within(SDL_Point point, gan::Transform2D& t);
    /// Moves the cursor into the frame of the actual scene considering borders.
    void adjust_to_relative_coords(gan::pos2 &point, gan::EngineContext &world);
    /// Uses the sweep method to detect collisions
    bool swept_aabb(const gan::box2& moving, const gan::box2& target, const gan::vec2& delta, float& toi, gan::vec2& normal);

    /// Converts a cstr to a float
    bool cstr_to_float(const char* str, float& out);
    /// Converts a cstr_to an integer
    bool cstr_to_int(const char* str, int& out);
    /// Checks if an input is a float
    bool is_float(const char* str);
    /// Checks if an input is an integer
    bool is_int(const char* str);

    /// vertex to string
    const char* to_cstr(SDL_Vertex v);
    /// fpoint to string
    const char* to_cstr(SDL_FPoint);
}

