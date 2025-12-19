#include "engine/scene/morphs/morph-types/Stretch.hpp"

#include "engine/scene/layers/LayerTime.hpp"

using namespace gfx;

Stretch::Stretch(geng::Gear *gear, float amplitude, float snappiness, float tolerance)
    : Morph(gear, amplitude, snappiness), snappiness(snappiness), tolerance(tolerance), oldPos(gear->t.pos){}


bool Stretch::update(geng::LayerTime& time) {
    geng::Gear& g = *gear;
    // Target-stretch x, target stretch y.
    float tsx, tsy;

    // First we check if we changed a considerable distance
    if (oldPos.dist(g.t.pos) > tolerance*1.5f) {
        // Then we calculate the total difference
        geng::Vertex diff = (g.t.pos - oldPos).unit().abs();
        // Modify our x and y accordingly
        tsx = (1 + diff[0]*3.0*amplitude/time.get_dt());
        tsy = (1 + diff[1]*3.0*amplitude/time.get_dt());
        // Then we calculate the normal of both
        float norm = 1/sqrtf(tsx*tsy);
        // In order to get a consistent area
        tsx *= norm;
        tsy *= norm;
    }
    else {
        // Otherwise, we set our targets to be the default values. (these go from 0->1)
        tsx = 1.f;
        tsy = 1.f;
    }
    // This turns our 0->1 calculation into an actual number
    float targetW = g.t.get_base_width()*tsx;
    float targetH = g.t.get_base_height()*tsy;
    // Snap if we're close
    if (std::abs(g.t.w - targetW) < 0.5f) g.t.w = targetW;
    // Otherwise we start approaching our target
    else {
        float mod = snappiness * 0.4f;
        if (mod < 0.1f) mod = 0.1f;
        g.t.w += (targetW - g.t.w) * mod;
    }
    // Snap if we're close
    if (std::abs(g.t.h - targetH) < 0.5f) g.t.h = targetH;
    // Otherwise approach the target
    else {
        float mod = snappiness * 0.4f;
        if (mod < 0.1f) mod = 0.1f;
        g.t.h += (targetH - g.t.h)  * mod;
    }
    // update our dpos tracker (change in position tracker)
    oldPos = g.t.pos;
    return false;
}
