#include "../../../../include/scene/render/RenderManager/RenderManager.hpp"
#include <iostream>

void RenderManager::update_canvas_size(){
    double wid, hei;
    // Gets the size of the window
    emscripten_get_element_css_size("canvas", &wid, &hei);

    int w = static_cast<int>(round(wid));
    int h = static_cast<int>(round(hei));
    // If our window changes we update shit
    if (w != canvasWidth || h != canvasHeight) {
        // Convert our w and h to ints to they go into width and height
        canvasWidth = w;
        canvasHeight = h;
        // Updates the window size
        SDL_SetWindowSize(window, canvasWidth, canvasHeight);
        // Tells our renderer to update its logic size.
        SDL_RenderSetLogicalSize(renderer, canvasWidth, canvasHeight);

        // Set our new camera width and height
        game::camera->set_width(canvasWidth);
        game::camera->set_height(canvasHeight);

        // Updates our scale accordingly
        scale = ((1.0 * canvasHeight) / canvasWidth < 0.75) ? 1.0*canvasHeight/scene::height : 1.0*canvasWidth/scene::width;
        if (scale < 1.0) scale = 1.0;
    }
}

// Returns our scene width.
int RenderManager::w() const {
    return canvasWidth;
}

// Returns our scene height.
int RenderManager::h() const {
    return canvasHeight;
}

// Constructor
RenderManager::RenderManager() : scale(1), canvasWidth(scene::width), canvasHeight(scene::height) {}

