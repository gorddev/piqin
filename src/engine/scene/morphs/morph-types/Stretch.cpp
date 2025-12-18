#include "engine/scene/morphs/morph-types/Stretch.hpp"

#include "engine/scene/layers/LayerTime.hpp"

using namespace gfx;

Stretch::Stretch(geng::Gear *gear, float amplitude, float snappiness)
    : Morph(gear, amplitude, snappiness), snappiness(snappiness), oldPos(gear->t.pos){}

bool Stretch::update(geng::LayerTime& time) {
    geng::Gear& g = *gear;
    // Target-stretch x, target stretch y.
    float tsx, tsy;
    geng::Vertex diff = (g.t.pos - oldPos).unit();

    if (oldPos.dist(g.t.pos) > 2.5f) {
        geng::Vertex diff = (g.t.pos - oldPos).unit().abs();
        tsx = (1 + diff[0]*snappiness*0.3f);
        tsy = (1 + diff[1]*snappiness*0.3f);
        float norm = 1/sqrtf(tsx*tsy);
        tsx *= norm;
        tsy *= norm;
    }
    else {
        tsx = 1.f;
        tsy = 1.f;
    }
    float targetW = g.t.get_base_width()*tsx;
    float targetH = g.t.get_base_height()*tsy;
    if (std::abs(g.t.w - targetW) < 0.5f) g.t.w = targetW;
    else {
        float mod = amplitude * 0.01f * time.get_dt();
        if (mod > 1.0f) mod = 1.0f;
        g.t.w += (targetW - g.t.w) * mod;
    }
    if (std::abs(g.t.h - targetH) < 0.5f) g.t.h = targetH;
    else {
        float mod = amplitude * 0.01f * time.get_dt();
        if (mod > 1.0f) mod = 1.0f;
        g.t.h += (targetH - g.t.h) * amplitude * 0.01f * mod;
    }
    oldPos = g.t.pos;
    return false;
}
