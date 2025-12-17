#pragma once

#include "engine/EngineContext.hpp"
#include "engine/types/Transform.hpp"
namespace gutils {

    constexpr float pi = 3.14159265358;
    constexpr float degreesToRadians = (pi / 180.0f);
    constexpr float roottwo = 1.4142135624;

    // Returns the sign of a variable to 4 digits (-1, 0, 1)
    signed char sgn(long double);

    /// Returns true if the point is within the transform.
    bool contained_within(SDL_Point point, geng::Transform& t);
    /// Moves the cursor into the frame of the actual scene considering borders.
    void adjust_to_relative_coords(SDL_Point &point, geng::EngineContext &world);
}
