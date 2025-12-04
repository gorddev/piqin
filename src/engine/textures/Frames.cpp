#include "engine/textures/Frame.hpp"
#include "engine/gengine-globals/EngineEnums.hpp"
#include "engine/utilities/Utilities.hpp"

using namespace gengine::textures;

// ................
// Frame Code
// ................

Frame::Frame(std::vector<SDL_FPoint> points, float duration, gengine::GENG_Anim anim)
    : vertexPoints(points),duration(duration), anim(anim) { }

void Frame::append_vertices(std::vector<SDL_Vertex>& buffer, Transform& t) {
    SDL_Vertex tl, tr, bl, br;
    // If our Transform isn't unlocked, we don't do anything fancy with it.
    tl = {{roundf(t.pos.x+t.w/2), roundf(t.pos.y)}, t.color, vertexPoints[0]};
    tr = {{roundf(t.pos.x + 3*t.w/2), roundf(t.pos.y)}, t.color, vertexPoints[1]};
    bl = {{roundf(t.pos.x), roundf(t.pos.y + t.h)}, t.color, vertexPoints[2]};
    br = {{roundf(t.pos.x + t.w), roundf(t.pos.y + t.h)}, t.color, vertexPoints[3]};
    // Commented out for speed purposes
    /*
    else {
        // Flipping if we choose to do so
        signed char flipX = (t.flipX) ? -1 : 1;
        signed char flipY = (t.flipY) ? -1 : 1;
        float ttw = flipX * t.w * t.scale;
        float tth = flipY * t.h * t.scale;

        // New texture coordinate if flipped?
        SDL_FPoint vpTL = vertexPoints[0];
        SDL_FPoint vpTR = vertexPoints[1];
        SDL_FPoint vpBL = vertexPoints[2];
        SDL_FPoint vpBR = vertexPoints[3];
        // If we flipped x
        if (t.flipX) {
            std::swap(vpTL, vpTR);
            std::swap(vpBL, vpBR);
        }
        // if we flipped y
        if (t.flipY) {
            std::swap(vpTL, vpBL);
            std::swap(vpTR, vpBR);
        }
        // If we aren't rotating
        if (t.angle < 0.1f) {
            tl = {{t.pos.x, t.pos.y}, t.color, vpTL};
            tr = {{t.pos.x + ttw, t.pos.y}, t.color, vpTR};
            bl = {{t.pos.x, t.pos.y + tth}, t.color, vpBL};
            br = {{t.pos.x + ttw, t.pos.y + tth}, t.color, vpBR};
        }
        else {
            float angle = t.angle * gengine::utils::degreesToRadians;
            // Get our adjusted center
            float rw = ttw * 0.5f;
            float rh = tth * 0.5f;
            // Our pivot point (the center)
            float px = t.pos.x + rw;
            float py = t.pos.y + rh;
            // Do a pre-calculation for cosine and sine
            float cs = cosf(angle);
            float sn = sinf(angle);
            // Create a lambda to rotate for us
            auto rotate = [&](float vx, float vy) {
                // First we get our x rotation (prabal)
                float rx = vx * cs - vy * sn;
                //Now our y
                float ry = vx * sn + vy * cs;
                // Return our point
                return SDL_FPoint{px + rx, py + ry};
            };
            // Now we just store it in our vertexes
            SDL_FPoint ftl = rotate(-rw, -rh);
            SDL_FPoint ftr = rotate(rw, -rh);
            SDL_FPoint fbl = rotate(-rw, rh);
            SDL_FPoint fbr = rotate(rw, rh);
            // Storing
            tl = {ftl, t.color, vpTL};
            tr = {ftr, t.color, vpTR};
            bl = {fbl, t.color, vpBL};
            br = {fbr, t.color, vpBR};
        }
    }
    */
    // Finally we push everything back on the buffer.
    buffer.push_back(tl);
    buffer.push_back(bl);
    buffer.push_back(br);
    buffer.push_back(tl);
    buffer.push_back(tr);
    buffer.push_back(br);
}


// ................
// Quad Code
// ................

Quad::Quad(int x, int y, int w, int h, float duration, GENG_Anim anim)
    : x(x), y(y), w(w), h(h), duration(duration), anim(anim) {}

Frame Quad::to_frame(int& texWidth, int& texHeight) const {
    float texW = static_cast<float>(texWidth);
    float texH = static_cast<float>(texHeight);
    // 0.5 prevents bleeding and weird rendering
    std::vector<SDL_FPoint> vertexPoints(4);
    vertexPoints[0] = {(x + 0.5f) / texW, (y + 0.5f) / texH}; // top-left
    vertexPoints[1] = {(x + w - 0.0f) / texW, (y + 0.5f) / texH}; // top-right
    vertexPoints[2] = {(x + 0.5f) / texW, (y + h - 0.0f) / texH}; // bottom-left
    vertexPoints[3] = {(x + w - 0.0f) / texW, (y + h - 0.0f) / texH}; // bottom-right
    return Frame(vertexPoints, duration, anim);
}


