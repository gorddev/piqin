#include "engine/scene/routes/route-types/Linear.hpp"

using namespace geng;
using namespace groute;

Linear::Linear(Gear *g, const Vertex &target, float speed)
    : Route(g, target, speed), direction((target-g->t.pos).unit()) {}

bool Linear::update(LayerTime& time) {
    // dereference for speed
    Transform& t = gear->t;

    if (overshoot(t.pos.x, target.x, speed*time.get_dt()*direction[0]))
        completeX = true;
    if (overshoot(t.pos.y, target.y, speed*time.get_dt()*direction[1]))
        completeY = true;
    if (overshoot(t.pos.z, target.z, speed*time.get_dt()*direction[2]))
        completeZ = true;

    return (completeX && completeY && completeZ);
}
