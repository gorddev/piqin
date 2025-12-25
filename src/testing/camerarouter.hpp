#pragma once
#include "engine/input/InputRouter.hpp"

class CameraRouter : public geng::InputRouter {
public:
    explicit CameraRouter(geng::InputRouterInit& init)
        : InputRouter(init) {}

    bool update(geng::Layer*& active_layer) override {
        glog::note << "camerarouter goes crazy." << glog::endlog;
        if (is_held(SDL_SCANCODE_LEFT)) {
            core.camera.pos.x -= 1;
        }
        if (is_held(SDL_SCANCODE_RIGHT)) {
            core.camera.pos.x += 1;
        }
        if (is_held(SDL_SCANCODE_DOWN)) {
            core.camera.pos.y += 1;
        }
        if (is_held(SDL_SCANCODE_UP)) {
            core.camera.pos.y -= 1;
        }
        return false;
    }


};
