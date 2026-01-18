#include "engine/scene/morphs/morph-types/Stretch.hpp"

#include "engine/layers/LayerState.hpp"

using namespace gfx;

Stretch::Stretch(gan::Gear *gear, float amplitude, float snappiness, float tolerance)
    : Morph(gear, -1, amplitude), snappiness(snappiness), tolerance(tolerance), oldPos(gear->t.pos){}


bool Stretch::update(gan::LayerState& time) {
    gan::Gear& g = *gear;
    // Target-stretch x, target stretch y.
    float tsx, tsy;

    gan::FPos2D diff = (g.t.pos - oldPos);
    tsx = 1 + std::min(fabsf(diff.x*amplitude)/time.get_dt(), 1.0*amplitude);
    tsy = 1 + std::min(fabsf(diff.y*amplitude)/time.get_dt(), 1.0*amplitude);


    float twidth = g.t.get_base_width()*tsx;
    float theight = g.t.get_base_height()*tsy;

    float area = g.t.get_base_width()*g.t.get_base_height();
    float mod = area/(twidth*theight);

    g.t.w += (twidth - g.t.w)*snappiness*mod;
    g.t.h += (theight - g.t.h)*snappiness*mod;

    oldPos = g.t.pos;

    return false;

    // First we check if we changed a considerable distance
    if (oldPos.dist(g.t.pos) > tolerance*1.5f) {
        // Then we calculate the total difference
        gan::FPos2D diff = (oldPos - g.t.pos).unit().abs();
        // Modify our x and y accordingly
        tsx = (1 + diff.x*3.0*amplitude/time.get_dt());
        tsy = (1 + diff.y*3.0*amplitude/time.get_dt());
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
