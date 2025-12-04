#pragma once
#include <SDL_render.h>
#include <vector>
#include "engine/gengine-globals/EngineEnums.hpp"
#include "engine/objects/Transform.hpp"

namespace gengine::textures {
    // Frame holds the vertices for rending, its duration, and the animation
    // You usually first make a quad and then turn it into a frame
    struct Frame {
    private:
        // Vertexes
        std::vector<SDL_FPoint> vertexPoints;

    public:
        // Duration
        float duration;
        GENG_Anim anim;         // Tells us whether to stay put,
        // Constructor for a frame
        Frame(std::vector<SDL_FPoint> points, float duration, GENG_Anim anim);
        // Gets our vertices for point x and y
        void append_vertices(std::vector<SDL_Vertex>& buffer, Transform& t);
    };

    // Lets us easily specify quads of the texture for a frame
    struct Quad {
        int x, y, w, h;
        float duration;
        GENG_Anim anim;
        Quad(int x, int y, int w, int h,
            float duration = 0.f, GENG_Anim anim = GENG_Anim::IDLE);
        Frame to_frame(int &texWidth, int &texHeight) const;
    };
}
