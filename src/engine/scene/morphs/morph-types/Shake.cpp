#include "engine/scene/morphs/morph-types/Shake.hpp"
#include <iostream>
#include "engine/utilities/Utilities.hpp"

using namespace gfx;
using namespace geng;

Shake::Shake(Gear* g, ShakeType shakeType, float amplitude, float speed, float duration, bool decay)
    : Morph(g, duration, amplitude), speed(speed), decay(decay) , shakeType(shakeType) {}

bool Shake::update(geng::LayerTime &time) {
    if (!permanent) {
        duration -= time.get_dt();
        if (duration <= 0) {
            gear->t.offset = {0.f, 0.f, 0.f};
            return true;
        }
    }

    // Create a reference do we don't need to dereference each time.
    Transform& tref = gear->t;

    if (shakeType == ShakeType::CIRCULAR)
        shake_circular(tref, time);
    else if (shakeType == ShakeType::FLOATY)
        shake_floaty(tref, time);
    else if (shakeType == ShakeType::RANDOM)
        shake_random(tref);
    else if (shakeType == ShakeType::SIDE)
        shake_side(tref, time);
    else
        complete = true;

    tref.offset *= (amplitude * ((decay) ? duration/initDuration : 1));
    return false;
}


void Shake::shake_random(Transform& tref) {
    tref.offset.x  = ((random()% 2001) /1000.0f) - 1;
    tref.offset.y  = ((random()% 2001) /1000.0f) - 1;
}

void Shake::shake_circular(Transform& tref, LayerTime& time) const {
    float div = time.get_time() * speed* 0.005f* 2*gutils::pi*((!permanent) ? 1 : 1);
    tref.offset.x = cosf(div);
    tref.offset.y = sinf(div);
}

void Shake::shake_floaty(Transform& tref, LayerTime& time) const {
    float input = time.get_time() * speed * 0.005;
    tref.offset.x = 0.333*cosf(input/1.4f) +0.1666*cosf(input/4.125f) + 0.5*sinf(input/4.414f);
    tref.offset.y = 0.5*sinf(input/2.2f) +0.25*sinf(input/4.3f) + 0.25*cosf(input/4.87f);
}

void Shake::shake_side(Transform& tref, LayerTime& time) const {
    if (tref.offset.is_zero())
        tref.offset.set((random()%2) ? 1 : -1,0,0);
    tref.offset.x = sinf(time.get_dt() * speed * 2.0f * gutils::pi);
    tref.offset.y = cosf(time.get_dt() * speed * 2.0f * gutils::pi);
}
