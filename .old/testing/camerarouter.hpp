#pragma once
#include "engine/input/InputRouter.hpp"

class CameraRouter : public gan::InputRouter {
public:
    explicit CameraRouter(gan::InputRouterInit& init)
        : InputRouter(init) {}

    bool update(gan::Layer*& active_layer, const uint8_t* keys) override {

        if (keys[SDL_SCANCODE_LEFT]) {
            active_layer->camera.pos.x -= 1;
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            active_layer->camera.pos.x += 1;
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            active_layer->camera.pos.y += 1;
        }
        if (keys[SDL_SCANCODE_UP]) {
            active_layer->camera.pos.y -= 1;
        }

        return false;
    }


};
