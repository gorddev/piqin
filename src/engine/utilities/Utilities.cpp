#include "engine/utilities/Utilities.hpp"

#include "engine/EngineContext.hpp"

signed char gutils::sgn(long double d) {
    const int dd = static_cast<int>(d * 1000.0) / 1000;
    if (dd > 0)
        return 1;
    if (dd < 0)
        return -1;
    return 0;
}

bool gutils::contained_within(SDL_Point point, geng::Transform &t) {
    return (point.x >= t.pos.x - t.get_base_width()/2.f && point.x <= t.pos.x + t.get_base_width()/2.f &&
            point.y >= t.pos.y - t.get_base_height()/2.f && point.y <= t.pos.y + t.get_base_height()/2.f);
}

void gutils::adjust_to_relative_coords(SDL_Point& point, geng::EngineContext& world) {
    point.x -= world.get_borderX();
    point.y -= world.get_borderY();
    point.x /= world.get_scale();
    point.y /= world.get_scale();
}