#include "engine/mods/plugins/routes/route-types/Balloon.hpp"

using namespace gan;
using namespace gfx;

Balloon::Balloon(Gear& g, const vec2 &target, float speed)
    : Route(g, target, speed), direction((target-g.t.pos).unit()) {}

bool Balloon::update(LayerState& time) {
    // dereference for speed
    Transform2D& t = gear.t;

    float DIST = target.dist(t.pos);
    vec2 dist = direction * time.get_dt() * speed * ((DIST < 1.0f) ? 1.0f : DIST) / 70.0f;
    //minimum step of movement
    float minny = (target.dist(t.pos) * 0.001f);
    const float minStep = std::max(minny, 0.001f);
    // This way we can preserve minimum stepping
    if (abs(dist.x) < minStep)
        dist.x = (direction.x >= 0.0f ? minStep : -minStep);
    if (abs(dist.y) < minStep)
        dist.y = (direction.y >= 0.0f ? minStep : -minStep);
    // Then we check if we overshoot or whether we're done or not!
    if (!(completeX & 1) && overshoot(t.pos.x, target.x, dist.x))
        completeX = true;
    if (!(completeX & 2) && overshoot(t.pos.y, target.y, dist.y))
        completeY = true;

    return (completeX && completeY);
}
