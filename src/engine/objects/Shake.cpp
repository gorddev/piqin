#include "engine/objects/Shake.hpp"
#include <iostream>
#include "engine/gengine-globals/scene.hpp"
#include "engine/utilities/Utilities.hpp"

using namespace gengine;

Shake::Shake(GENG_Shake shakeType, float strength, float duration, float speed, bool decay)
    : shakeType(shakeType), strength(strength), speed(speed), duration(duration), decay(decay) {
    displacement = Vertex(0,0,0);
    complete = false;
    initDuration = duration;
    if (duration == -1)
        stop = false;
}

Vertex Shake::shake_it() {
    if (complete && stop)
        return {0.0f,0.0f,0.0f};
    if (shakeType == GENG_Shake::CIRCULAR)
        shake_circular();
    else if (shakeType == GENG_Shake::FLOATY)
        shake_floaty();
    else if (shakeType == GENG_Shake::RANDOM)
        shake_random();
    else if (shakeType == GENG_Shake::SIDE)
        shake_side();
    else
        complete = true;

    if (!stop)
        return displacement * strength;
    duration -= glb::scene.dt;
    if (duration <= 0)
        complete = true;
    return displacement * (strength * ((decay) ? duration/initDuration : 1));
}

bool Shake::done() const {
    return complete;
}

void Shake::end() {
    complete = true;
    duration = 0;
}

bool Shake::infinite() {
    return (duration == -1);
}

Vertex& Shake::get_displacement() {
    return displacement;
}

void Shake::shake_random() {
    displacement.x  = ((random()% 2001) /1000.0f) - 1;
    displacement.y  = ((random()% 2001) /1000.0f) - 1;
}

void Shake::shake_circular() {
    float div = glb::scene.time * speed* 0.005f* 2*utils::pi*((!stop) ? 1 : 1);
    displacement.x = cosf(div);
    displacement.y = sinf(div);
}

void Shake::shake_floaty() {
    float input = glb::scene.time * speed * 0.005;
    displacement.x = 0.333*cosf(input/1.4f) +0.1666*cosf(input/4.125f) + 0.5*sinf(input/4.414f);
    displacement.y = 0.5*sinf(input/2.2f) +0.25*sinf(input/4.3f) + 0.25*cosf(input/4.87f);
}

void Shake::shake_side() {
    if (displacement.zero())
        displacement.set((random()%2) ? 1 : -1,0,0);
    displacement.x = sinf(glb::scene.time * speed * 2.0f * M_PI);
    displacement.y = cosf(glb::scene.time * speed * 2.0f * M_PI);
}
