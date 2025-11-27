#include "generics/Shake.hpp"

#include <iostream>

#include "Constants.hpp"

Shake::Shake(uint8_t shakeType, float strength, int duration, float speed, bool decay)
    : shakeType(shakeType), strength(strength), speed(speed), duration(duration), decay(decay) {
    displacement = Vertex(0,0,0);
    complete = false;
    popPos.set(-1,-1,-1);
    initDuration = duration;
}

bool Shake::shake_it(Vertex &pos) {
    if (complete)
        return true;
    if (shakeType == SHAKE_CIRCULAR)
        shake_circular();
    else if (shakeType == SHAKE_FLOATY)
        shake_floaty();
    else if (shakeType == SHAKE_RANDOM)
        shake_random();
    else if (shakeType == SHAKE_SIDE)
        shake_side();
    else
        complete = true;

    // Adds our displacement and applies decay if necessary

    if (popPos.x >=0)
        pos = popPos + displacement * strength * ((decay) ? duration/initDuration: 1);
    else
        pos += displacement * strength*((decay) ? duration/initDuration: 1);
    duration -= scene::dt;
    if (duration <= 0)
        return true;
    return false;

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
    float input = scene::time * speed * SHAKE_CIRCULAR_SPEED;
    displacement.x = cosf(input);
    displacement.y = sinf(input);
}

void Shake::shake_floaty() {
    float input = scene::time * speed * SHAkE_FLOATY_SPEED;
    displacement.x = 0.5*cosf(input/2.0f) +0.25*cosf(input/4.0f) + 0.25*sinf(input/4.0f);
    displacement.y = 0.5*sinf(input/2.0f) +0.25*sinf(input/4.0f) + 0.25*cosf(input/4.0f);
}

void Shake::shake_side() {
    if (displacement.zero())
        displacement.set(1,0,0);
    displacement.x = -1*displacement.x;
    displacement.y = -1*displacement.y;
}
