#include "engine/render/RenderManager.hpp"
#include "engine/utilities/Utilities.hpp"

/*******************/
/* HELPER FUNCTIONS */
/*******************/

using namespace gengine;

// Calculates the rectangle position for flat displays on canvas
SDL_FRect RenderManager::rect_flat(Object& o) const {
    SDL_FRect f = {o.x() + o.offset().x, o.y() + o.offset().y, o.width()*o.scale(), o.height()*o.scale()};
    return rect_flat(f);
}

SDL_FRect RenderManager::rect_flat(SDL_FRect f) const {
    f.x -= (f.w * 0.5f);
    f.y -= (f.h * 0.5f);
    return f;
}

SDL_FRect RenderManager::rect_shadow(Object& o) {
    // Only difference between rect_vertical is that shadows get engine::smaller with z distance
    SDL_FRect s = {o.x() + o.offset().x, o.y() + o.offset().y, static_cast<float>(o.width()), static_cast<float>(o.height())};
    return rect_shadow(s);
}

SDL_FRect RenderManager::rect_shadow(SDL_FRect f) {
    f.x -= (f.w / 2);
    f.y -= (f.h / 2);
    SDL_FRect offset = rect_shadow_offset(f);
    f.x += offset.x;
    f.y += offset.y;
    f.w += offset.w;
    f.h += offset.h;
    return f;
}

SDL_FRect RenderManager::rect_shadow_offset(SDL_FRect s) {
    float dx = 0.02f*(s.x - (glb::scene.width*0.5f));
    float dy = 0.02f*(s.y - glb::scene.height*0.25f);
    // Then we scale the shadows
    float scl = sqrt((dx*dx) + (dy*dy)) * 0.005f;
    float dw = (s.w * scl);
    float dh = (s.h * scl);
    return {dx, dy, dw, dh};
}