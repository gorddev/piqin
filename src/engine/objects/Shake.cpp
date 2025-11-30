#include "engine/objects/Shake.hpp"
#include <iostream>
#include "engine/gengine-globals/scene.hpp"
#include "engine/utilities/Utilities.hpp"

using namespace gengine;

Shake::Shake(ShakeType shakeType, float strength, float duration, float speed, bool decay)
    : shakeType(shakeType), strength(strength), speed(speed), duration(duration), decay(decay) {
    displacement = Vertex(0,0,0);
    complete = false;
    popPos.set(-1,-1,-1);
    if (duration == -1)
        this->duration = 1000000000000.0f;
    initDuration = duration;
}

bool Shake::shake_it(Vertex &pos) {
    if (complete)
        return true;
    if (shakeType == ShakeType::CIRCULAR)
        shake_circular();
    else if (shakeType == ShakeType::FLOATY)
        shake_floaty();
    else if (shakeType == ShakeType::RANDOM)
        shake_random();
    else if (shakeType == ShakeType::SIDE)
        shake_side();
    else
        complete = true;

    // Adds our displacement and applies decay if necessary

    if (popPos.x >=0)
        pos = popPos + displacement * strength * ((decay) ? duration/initDuration: 1);
    else
        pos += displacement * strength*((decay) ? duration/initDuration: 1);
    duration -= glb::scene.dt;
    if (duration <= 0)
        return true;
    return false;
}

bool Shake::infinite() {
    return (duration == -1);
}

void Shake::set_pos_pop(Vertex&v) {
    popPos = v;
}

Vertex Shake::pop_pos() {
    return popPos;
}

void Shake::shake_random() {
    displacement.x  = ((random()% 2001) /1000.0f) - 1;
    displacement.y  = ((random()% 2001) /1000.0f) - 1;
}

void Shake::shake_circular() {
    float div = speed*2*utils::pi*duration/initDuration;
    displacement.x = cosf(div);
    displacement.y = sinf(div);
}

void Shake::shake_floaty() {
    float input = glb::scene.time * speed * 0.005;
    displacement.x = 0.5*cosf(input/2.0f) +0.25*cosf(input/4.0f) + 0.25*sinf(input/4.0f);
    displacement.y = 0.5*sinf(input/2.0f) +0.25*sinf(input/4.0f) + 0.25*cosf(input/4.0f);
}

void Shake::shake_side() {
    if (displacement.zero())
        displacement.set(1,0,0);
    displacement.x = -1*displacement.x;
    displacement.y = -1*displacement.y;
}
