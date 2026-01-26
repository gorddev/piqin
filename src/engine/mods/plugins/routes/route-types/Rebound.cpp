#include "engine/mods/plugins/routes/route-types/Rebound.hpp"

using namespace gan;
using namespace groute;

Rebound::Rebound(Gear& g, const vec2 &target, float speed)
    : Route(g, target, speed) {}

bool Rebound::update(LayerState& time) {
    //deref
    Transform2D& t = gear.t;

    speed += 0.025f * ((fabsf(speed) > 1.0f) ? sqrtf(fabsf(speed)) : 1.0f);
    vec2 dist = start * (speed * -20.f);
    vec2 direction = (target-start).unit();

    if (speed > 0.0f) {
        if (overshoot(t.pos.x, target.x, speed*time.get_dt()*direction.x))
            completeX = true;
        if (overshoot(t.pos.y, target.y, speed*time.get_dt()*direction.y))
            completeY = true;

        return (completeX && completeY);
    }
    t.pos += dist;
    return false;
}
