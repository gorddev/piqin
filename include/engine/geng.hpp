#pragma once

#include "core/Engine.hpp"

namespace geng {
    // holds data user can make
    struct LoopData {
        geng::Engine* engine;
        std::function<bool()> userfunc;
    };

    // internal loop which updates the engine
    inline bool geng_internal_game_loop(double time, void* userdata) {
        auto* data = static_cast<LoopData*>(userdata);
        Engine& eng = *data->engine;
        // Tick the engine
        eng.tick(time);
        // Call the user's function
        bool continue_loop = (data->userfunc)();
        // Update and render
        eng.update();
        eng.render();
        return continue_loop;
    }
} // namespace geng

// starts the primary game loop in main
#define GENG_START_LOOP(engine, user_lambda) \
do {   \
    static std::function<bool()> lambda = user_lambda; \
    static geng::LoopData loop_data { &engine, lambda }; \
    emscripten_request_animation_frame_loop( \
        geng::geng_internal_game_loop, &loop_data  \
    );  \
    emscripten_exit_with_live_runtime(); \
} while(0)