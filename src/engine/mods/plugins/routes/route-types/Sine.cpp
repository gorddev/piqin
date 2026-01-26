#include "engine/mods/plugins/routes/route-types/Sine.hpp"
#include "engine/utilities/Utilities.hpp"

using namespace gan;
using namespace groute;

Sine::Sine(Gear& g, const vec2 &target, float speed)
    : Route(g, target, speed), direction((target-g.t.pos).unit()) {}

bool Sine::update(LayerState& time) {
    //deref
    Transform2D& t = gear.t;

    // real distance-based time instead of x-based time
    float distTraveled = (t.pos - start).mag();
    float totalDist = (target - start).mag();

    float ttime = distTraveled / speed;
    float totalTime = totalDist / speed;

    // Linear base motion
    t.pos += direction * speed * time.get_dt() * 0.5;

    // Apply sine only to y
    float sineOffset = 10 * sinf(2.0f * gutils::pi * ttime / totalTime);
    if (distTraveled < 0.001f)
        t.pos.y += direction.y * distTraveled + sineOffset;
    else
        t.pos.y = start.y + direction.y * distTraveled + sineOffset;

    // Overshoot on x/z only
    if (overshoot(t.pos.x, target.x, direction.x * time.get_dt() * speed))
        completeX = true;;
    // We dont need to check for y because otherwise itll bmess with movement.
    completeY = true;

    return (completeX);
}
