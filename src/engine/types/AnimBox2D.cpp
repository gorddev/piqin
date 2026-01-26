#include "../../../include/engine/types/AnimBox2D.hpp"

#include <sstream>
#include "../../../include/engine/mods/animation/frames/Frame.hpp"

using namespace gan;

// ................
// AnimBox2D Code
// ................

AnimBox2D::AnimBox2D(int x, int y, int w, int h, float duration, GAnimType anim, short next_anim)
    : x(x), y(y), w(w), h(h), duration(duration), anim(anim), next_anim(next_anim) {}

std::vector<SDL_FPoint> AnimBox2D::to_vert_points(int texW, int texH) const{
    std::vector<SDL_FPoint> vertexPoints(4);
    float u0 = (x - 0.f) / texW;
    float v0 = (y - 0.f) / texH;
    float u1 = (x + w + 0.f) / texW;
    float v1 = (y + h + 0.f) / texH;
    vertexPoints[0] = {u0, v0};
    vertexPoints[1] = {u1, v0};
    vertexPoints[2] = {u0, v1};
    vertexPoints[3] = {u1, v1};
    return vertexPoints;
}

std::string AnimBox2D::to_string() const {
    std::stringstream ss;
    ss << "Width: " << w << " Height: " << h << " X:" << x << " Y:" << y;
    return ss.str();
}
