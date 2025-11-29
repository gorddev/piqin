#include "../../../../include/scene/render/RenderManager/RenderManager.hpp"

/*******************/
/* HELPER FUNCTIONS */
/*******************/

// Calculates the rectangle position for flat displays on canvas
SDL_FRect RenderManager::rect_flat(Render_Rect canvas) const {
    canvas.x -= canvas.w / 2.0 + game::camera->pos.x;
    canvas.y += canvas.h / 2.0 + game::camera->pos.y;
    return canvas.to_SDL_FRect();
}

SDL_FRect RenderManager::rect_shadow(Render_Rect canvas, const double z) const {
    // Only difference between rect_vertical is that shadows get engine::smaller with z distance
    double dx = (canvas.x - (scene::width/2.0));
    double dy = (canvas.y - (scene::height/2.0));
    canvas.x -= static_cast<int>(canvas.w / 2.0) + game::camera->pos.x;
    canvas.y += static_cast<int>(canvas.h / 2.0) + game::camera->pos.y;
    canvas.x += static_cast<int>(dx*0.055);
    canvas.y += static_cast<int>(dy*0.055);

    // Then we scale the shadows
    double scale = sqrt(sqrt((dx*dx) + (dy*dy))) * 0.005;
    canvas.w *= 1 + scale;
    canvas.h *= 1 + scale;

    return canvas.to_SDL_FRect();
}