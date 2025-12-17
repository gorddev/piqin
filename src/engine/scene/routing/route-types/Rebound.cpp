#include "engine/scene/routes/route-types/Rebound.hpp"

using namespace geng;
using namespace groute;

Rebound::Rebound(Gear *g, const Vertex &target, float speed)
    : Route(g, target, speed) {}

bool Rebound::update(LayerTime& time) {
    //deref
    Transform& t = gear->t;

    speed += 0.025f * ((fabsf(speed) > 1.0f) ? sqrtf(fabsf(speed)) : 1.0f);
    Vertex dist = start * (speed * -20.f);
    Vertex direction = (target-start).unit();

    if (speed > 0.0f) {
        if (overshoot(t.pos.x, target.x, speed*time.get_dt()*direction[0]))
            completeX = true;
        if (overshoot(t.pos.y, target.y, speed*time.get_dt()*direction[1]))
            completeY = true;
        if (overshoot(t.pos.z, target.z, speed*time.get_dt()*direction[2]))
            completeZ = true;

        return (completeX && completeY && completeZ);
    }
    t.pos += dist;
    return false;
}
