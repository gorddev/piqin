#include "engine/render/RenderManager.hpp"

/*******************/
/* HELPER FUNCTIONS */
/*******************/

using namespace gengine;

// Calculates the rectangle position for flat displays on canvas
SDL_FRect RenderManager::rect_flat(Object& o) const {
    float x = o.x() - (o.width() / 2.0 + cam->pos.x) + o.offset().x;
    float y = o.y() + (o.height() / 2.0 + cam->pos.y) + o.offset().y;
    return {x, y, (o.width() * o.scale()), (o.height() * o.scale())};
}

SDL_FRect RenderManager::rect_shadow(Object& o) const {
    // Only difference between rect_vertical is that shadows get engine::smaller with z distance
    float dx = (o.x() - (glb::scene.width/2.0));
    float dy = (o.y() - (glb::scene.height/2.0));
    float px = o.x() - (o.width() / 2) - cam->pos.x + o.offset().x;
    float py = o.y() + (o.height() / 2) - cam->pos.y + o.offset().y;
    px += static_cast<int>(dx*0.055);
    py += static_cast<int>(dy*0.055);

    // Then we scale the shadows
    float scl = sqrtf(sqrt((dx*dx) + (dy*dy))) * 0.005;
    float w = o.width() * (1 + scl);
    float h = o.height() * (1 + scl);

    return {px, py, w, h};
}