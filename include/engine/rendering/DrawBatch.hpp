#pragma once
#include <cstdint>
#include <SDL_render.h>

namespace geng {

    /** The Layers send DrawBatches to the renderer so the renderer can batch render calls
     * Because Layers have access to their own textures, they send them to the renderer so they renderer knows what to do.*/
    struct DrawBatch {
        int num_vertices = 0;
        int start_index;
        int texture_id = -1;
        SDL_Texture* tex;
        DrawBatch(SDL_Texture* tex, int start_index)
            : start_index(start_index), tex(tex) {}

        int size() const {
            return num_vertices;
        }
        SDL_Texture* texture() const {
            return tex;
        }
        int begin() const {
            return start_index;
        }

    };
}
