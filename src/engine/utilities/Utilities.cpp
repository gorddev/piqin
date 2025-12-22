#include "engine/utilities/Utilities.hpp"

#include "engine/EngineContext.hpp"

using namespace gutils;
using namespace geng;

signed char gutils::sgn(long double d) {
    const int dd = static_cast<int>(d * 1000.0) / 1000;
    if (dd > 0)
        return 1;
    if (dd < 0)
        return -1;
    return 0;
}

bool gutils::contained_within(SDL_Point point, geng::Transform2D &t) {
    return (point.x >= t.pos.x - t.get_base_width()/2.f && point.x <= t.pos.x + t.get_base_width()/2.f &&
            point.y >= t.pos.y - t.get_base_height()/2.f && point.y <= t.pos.y + t.get_base_height()/2.f);
}

void gutils::adjust_to_relative_coords(SDL_Point& point, geng::EngineContext& world) {
    point.x -= world.get_borderX();
    point.y -= world.get_borderY();
    point.x /= world.get_scale();
    point.y /= world.get_scale();
}

// not my function, just found it.
bool swept_aabb(const Box2D& moving, const Box2D& target, const FPos2D& delta, float& toi, FPos2D& normal) {
    if (delta.x == 0.0f && delta.y == 0.0f)
        return false;

    float xInvEntry, yInvEntry;
    float xInvExit,  yInvExit;

    if (delta.x > 0.0f) {
        xInvEntry = target.x - (moving.x + moving.w);
        xInvExit  = (target.x + target.w) - moving.x;
    } else {
        xInvEntry = (target.x + target.w) - moving.x;
        xInvExit  = target.x - (moving.x + moving.w);
    }

    // Y axis
    if (delta.y > 0.0f) {
        yInvEntry = target.y - (moving.y + moving.h);
        yInvExit  = (target.y + target.h) - moving.y;
    } else {
        yInvEntry = (target.y + target.h) - moving.y;
        yInvExit  = target.y - (moving.y + moving.h);
    }

    float xEntry, yEntry;
    float xExit,  yExit;

    if (delta.x == 0.0f) {
        xEntry = -INFINITY;
        xExit  =  INFINITY;
    } else {
        xEntry = xInvEntry / delta.x;
        xExit  = xInvExit  / delta.x;
    }

    if (delta.y == 0.0f) {
        yEntry = -INFINITY;
        yExit  =  INFINITY;
    } else {
        yEntry = yInvEntry / delta.y;
        yExit  = yInvExit  / delta.y;
    }

    float entryTime = std::max(xEntry, yEntry);
    float exitTime  = std::min(xExit,  yExit);

    // Reject
    if (entryTime > exitTime ||
        (xEntry < 0.0f && yEntry < 0.0f) ||
        xEntry > 1.0f || yEntry > 1.0f)
        return false;

    toi = std::clamp(entryTime, 0.0f, 1.0f);
    normal = {0.0f, 0.0f};

    if (xEntry > yEntry)
        normal.x = (delta.x < 0.0f) ? 1.0f : -1.0f;
    else
        normal.y = (delta.y < 0.0f) ? 1.0f : -1.0f;

    return true;
}