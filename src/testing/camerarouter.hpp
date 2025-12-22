#pragma once
#include "engine/input/InputRouter.hpp"

class CameraRouter : public geng::InputRouter {
public:
    CameraRouter() = default;

    bool update(geng::Layer*& active_layer) override {
        if (is_held(SDL_SCANCODE_LEFT)) {
            active_layer->scene.camera.pos.x -= 1;
        }
        if (is_held(SDL_SCANCODE_RIGHT)) {
            active_layer->scene.camera.pos.x += 1;
        }
        if (is_held(SDL_SCANCODE_DOWN)) {
            active_layer->scene.camera.pos.y += 1;
        }
        if (is_held(SDL_SCANCODE_UP)) {
            active_layer->scene.camera.pos.y -= 1;
        }
        return false;
    }


};
