#include "engine/scene/routes/route-types/Linear.hpp"

using namespace gan;
using namespace groute;

Linear::Linear(Gear& g, const FPos2D &target, float speed)
    : Route(g, target, speed), direction((target-g.t.pos).unit()) {}

bool Linear::update(LayerState& time) {
    // dereference for speed
    Transform2D& t = gear.t;

    if (overshoot(t.pos.x, target.x, speed*time.get_dt()*direction.x))
        completeX = true;
    if (overshoot(t.pos.y, target.y, speed*time.get_dt()*direction.y))
        completeY = true;

    return (completeX && completeY && completeZ);
}
