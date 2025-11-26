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
    double origx = canvas.x;
    double origy = canvas.y;
    canvas.x -= canvas.w / 2.0 + game::camera->pos.x;
    canvas.y += canvas.h / 2.0 + game::camera->pos.y;
    canvas.x += (origx - (scene::width/2.0))*0.055;
    canvas.y += (origy - (scene::height/2.0))*0.055;

    return canvas.to_SDL_FRect();
}