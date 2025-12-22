#include "engine/scene/routes/route-types/Torpedo.hpp"


using namespace geng;
using namespace groute;

Torpedo::Torpedo(Gear &g, const FPos2D &target, float speed)
    : Route(g, target, speed), direction((target-g.t.pos).unit()) {}

bool Torpedo::update(LayerState& time) {
    // Deref for speed
    Transform2D& t = gear.t;
    //calculate our unit vector
    FPos2D unit = (target - t.pos).unit();
    FPos2D dist = unit * time.get_dt() * speed * (1.0f/target.dist(t.pos)) * 0.10f;
    if (dist.mag() < 0.00001f) { return true; }
    //minimum step of movement
    const float minStep = 0.0025f * target.dist(t.pos);
    // This way we can preserve minimum stepping
    if (fabs(dist.x) < minStep)
        dist.x = (unit.x >= 0.0f ? minStep : -minStep);
    if (fabs(dist.y) < minStep)
        dist.y = (unit.y >= 0.0f ? minStep : -minStep);
    // Then we check if we overshoot or whether we're done or not!
    if (!(completeX & 1) && overshoot(t.pos.x, target.x, dist.x))
        completeX = true;
    if (!(completeX & 2) && overshoot(t.pos.y, target.y, dist.y))
        completeY = true;

    return (completeX && completeY);
}
