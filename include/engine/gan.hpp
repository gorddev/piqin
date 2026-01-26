#pragma once

#include "core/Engine.hpp"

namespace gan {
    /** Container used to hold engine and user data so the macro GENG_START_LOOP works as intended**/
    struct LoopData {
        gan::Engine* engine;
        std::function<bool()> userfunc;
    };

    inline uint64_t freq = SDL_GetPerformanceFrequency();

    // internal loop which updates the engine
    inline bool gan_internal_game_loop(double time, void* userdata) {
        auto* data = static_cast<LoopData*>(userdata);
        Engine& eng = *data->engine;

        uint64_t frame_start = SDL_GetPerformanceCounter();

        // Tick the engine
        if (!eng.tick(time)) {
            eng.shutdown();
            return false;
        }

        // Call the user's function
        bool continue_loop = (data->userfunc)();
        // Update and render
        eng.update();
        eng.render();

        static double target_dt = 0.008;


        uint64_t frame_end = SDL_GetPerformanceCounter();
        double frame_time = (double)(frame_end - frame_start) / freq;

        if (frame_time < target_dt) {
            double remaining = target_dt - frame_time;

            if (remaining > 0.002) {
                SDL_Delay(static_cast<Uint32>((remaining-0.001)*1000));
            }

            while (true) {
                double elapsed =
                    (double)(SDL_GetPerformanceCounter() - frame_start) / freq;
                if (elapsed >= target_dt)
                    break;
                SDL_PumpEvents();
            }
        }

        return continue_loop;
    }
}

// starts the primary game loop in main
#define GENG_START_LOOP(engine, user_lambda) \
do {   \
    static std::function<bool()> lambda = user_lambda; \
    static gan::LoopData loop_data { &engine, lambda }; \
    while (  \
        gan::gan_internal_game_loop(SDL_GetTicks64(), &loop_data) \
    );  \
} while(0);