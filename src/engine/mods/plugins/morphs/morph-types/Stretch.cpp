#include "engine/mods/plugins/morphs/morph-types/Stretch.hpp"

#include "engine/layers/layer-subobjects/LayerState.hpp"

using namespace gfx;

Stretch::Stretch(gan::Gear *gear, StretchInfo stretch_info)
    : Morph(gear, -1), s(stretch_info)
{
    // Make sure our amplitude is greater than 0
    if (amplitude < 0)
        amplitude = 0;
    // Establish a stretching direction
    stretch_dir = (amplitude > 1) ? 1 : -1;
    // set our old pos to the gear's pos
    oldPos = gear->t.pos;
}


bool Stretch::update(gan::LayerState& time) {
    gan::Gear& g = *gear;
    // Get relative snap
    float dt = static_cast<float>(time.get_dt());
    float snap_delta = s.snappiness * dt / 8.f;
    float rebound_delta = s.rebound * dt / 32.f;

    gan::vec2 diff = (g.t.pos - oldPos).abs();
    // If we meaningfully change x

    auto stretch_func = [&](Mode& state, float& dif, const uint16_t base, float& dim) {
        if (dif > s.tolerance)
            state = STRETCH_IN;
        if (state == STRETCH_IN) {
            dim += snap_delta * stretch_dir * dif/s.tolerance;
            if (stretch_dir < 0) {
                if (dim < s.amplitude*base) {
                    dim = s.amplitude*base;
                    state = STRETCH_OUT;
                }
            }
            else if (stretch_dir > 0) {
                if (dim > s.amplitude*base) {
                    dim = s.amplitude*base;
                    state = STRETCH_OUT;
                }
            }
        }
        if (state == STRETCH_OUT) {
            dim -= (dim-base) * rebound_delta;
            if (fabs(dim-base) <= rebound_delta) {
                dim = base;
                state = STRETCH_NONE;
            }
        }
    };

    // stretch x
    stretch_func(state_x, diff.x, g.t.get_base_height(), g.t.h);
    // stretch y
    stretch_func(state_y, diff.y, g.t.get_base_width(), g.t.w);
    // update previous position
    oldPos = g.t.pos;
    return false;
}
