#include "engine/scene/routes/route-types/Torpedo.hpp"


using namespace geng;
using namespace groute;

Torpedo::Torpedo(Gear *g, const Vertex &target, float speed)
    : Route(g, target, speed), direction((target-g->t.pos).unit()) {}

bool Torpedo::update(LayerTime& time) {
    // Deref for speed
    Transform& t = gear->t;
    //calculate our unit vector
    Vertex unit = (target - t.pos).unit();
    Vertex dist = unit * time.get_dt() * speed * (1.0f/target.dist(t.pos)) * 0.10f;
    if (dist.mag() < 0.00001f) { return true; }
    //minimum step of movement
    const float minStep = 0.0025f * target.dist(t.pos);
    // This way we can preserve minimum stepping
    for (int i = 0; i < 3; ++i) {
        if (fabs(dist[i]) < minStep) {
            dist[i] = (unit.x >= 0.0f ? minStep : -minStep);
        }
    }
    // Then we check if we overshoot or whether we're done or not!
    if (!(completeX & 1) && overshoot(t.pos.x, target.x, dist[0]))
        completeX = true;
    if (!(completeX & 2) && overshoot(t.pos.y, target.y, dist[1]))
        completeY = true;
    if (!(completeX & 4) && overshoot(t.pos.z, target.z, dist[2]))
        completeZ = true;

    return (completeX && completeY && completeZ);
}
