#include "engine/mods/animation/frames/Frame.hpp"
#include "engine/utilities/Utilities.hpp"

using namespace gan;

// ................
// Frame Code
// ................

Frame::Frame(AnimBox2D anibox, IMG_Info& info)
    : vertexPoints(anibox.to_vert_points(info.w, info.h)),duration(anibox.duration),
        anim(anibox.anim), next_anim(anibox.next_anim) {
}
void Frame::append_vertices(RenderBuffer& buffer, Gear* gear) {
    Gear& g = *gear;
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

    float scaledW = g.t.w * g.t.scale;
    float scaledH = g.t.h * g.t.scale;

    float lx = (g.t.pos.x + g.t.offset.x) - scaledW * 0.5f;
    float rx = lx + scaledW;
    float ty = (g.t.pos.y + g.t.offset.y) - scaledH * 0.5f;
    float by = ty + scaledH;

    SDL_Vertex tl{{lx, ty}, g.t.color, uvTL };
    SDL_Vertex tr{ {rx, ty}, g.t.color, uvTR };
    SDL_Vertex bl{ {lx, by}, g.t.color, uvBL };
    SDL_Vertex br{ {rx,by}, g.t.color, uvBR };

    buffer.push_back(tl);
    buffer.push_back(bl);
    buffer.push_back(br);

    buffer.push_back(tl);
    buffer.push_back(br);
    buffer.push_back(tr);
}

float Frame::get_duration() const {
    return duration;
}

gan::GAnimType Frame::get_anim_type() const {
    return anim;
}

short Frame::get_next_anim() const {
    return next_anim;
}



