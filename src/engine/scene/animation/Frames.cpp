#include "engine/scene/animation/Frame.hpp"
#include "../../../../include/engine/core/gears/GearFlag.hpp"
#include "engine/utilities/Utilities.hpp"

using namespace geng;

// ................
// Frame Code
// ................

Frame::Frame(AnimBox2D anibox, IMG_Info& info)
    : vertexPoints(anibox.to_vert_points(info.w, info.h)),duration(anibox.duration),
        anim(anibox.anim), next_anim(anibox.next_anim) {
}
void Frame::append_vertices(RenderBuffer& buffer, Gear* gear) {
    Gear& g = *gear;
    const float hw = g.t.w * g.t.scale * 0.5f;
    const float hh = g.t.h * g.t.scale * 0.5f;
    const float cx = g.t.pos.x + g.t.offset.x;
    const float cy = g.t.pos.y + g.t.offset.y;
    SDL_FPoint uvTL = vertexPoints[0];
    SDL_FPoint uvTR = vertexPoints[1];
    SDL_FPoint uvBL = vertexPoints[2];
    SDL_FPoint uvBR = vertexPoints[3];

    if (g.flippedX()) {
        std::swap(uvTL, uvTR);
        std::swap(uvBL, uvBR);
    }
    if (g.flippedY()) {
        std::swap(uvTL, uvBL);
        std::swap(uvTR, uvBR);
    }

    // Local-space corners
    SDL_FPoint ltl{ -hw, -hh };
    SDL_FPoint ltr{  hw, -hh };
    SDL_FPoint lbl{ -hw,  hh };
    SDL_FPoint lbr{  hw,  hh };

    const float angle = g.t.angle;

    auto to_world = [&](const SDL_FPoint& p) -> SDL_FPoint {
        if (std::abs(angle) < 0.0001f) {
            return { cx + p.x, cy + p.y };
        }

        float a  = angle * gutils::degreesToRadians;
        float cs = std::cos(a);
        float sn = std::sin(a);

        return {
            cx + (p.x * cs - p.y * sn),
            cy + (p.x * sn + p.y * cs)
        };
    };

    SDL_Vertex tl{ to_world(ltl), g.t.color, uvTL };
    SDL_Vertex tr{ to_world(ltr), g.t.color, uvTR };
    SDL_Vertex bl{ to_world(lbl), g.t.color, uvBL };
    SDL_Vertex br{ to_world(lbr), g.t.color, uvBR };

    // Two triangles (consistent winding)
    buffer.push_back(tl);
    buffer.push_back(bl);
    buffer.push_back(br);

    buffer.push_back(tl);
    buffer.push_back(tr);
    buffer.push_back(br);
}
float Frame::get_duration() const {
    return duration;
}

geng::GAnimType Frame::get_anim_type() const {
    return anim;
}

short Frame::get_next_anim() const {
    return next_anim;
}



