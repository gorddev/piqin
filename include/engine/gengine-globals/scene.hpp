#pragma once
#include <cstdint>

namespace gengine {
    // Scene information
    struct Scene {
    private:
        float prevTime = 0.0f;
    public:
        // Game timing
        float dt = 0.0f;
        float time = 0.0f;
        uint64_t frame = 0;
        // Scene width, and height
        int width = 340;
        int height = 200;
        // Main menu?
        bool mainMenu = false;

        void update(float time) {
            this->time = time;
            dt = time - prevTime;
            prevTime = time;
            frame++;
        }
    };

    namespace glb {
        inline Scene scene;
    }
    // Fading in/out
    struct fader {
        // Member variables
        bool active = false;
        uint8_t faderAlpha = 80;
        uint8_t fadeStyle = 0;
    };
}
