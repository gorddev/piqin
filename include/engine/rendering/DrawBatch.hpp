#pragma once

#include <SDL_render.h>

namespace gan {

    /** @brief The Layers send DrawBatches to the renderer so the renderer can batch render calls.
     * @details DrawBuffer has the following member functions:
     * - @code num_vertices@endocde › THe number of vertices in the batch
     * - @code start_index@endcode › The starting index in terms of the buffer where the batch starts
     * - @code texture_id@endcode › The current texture if of the render buffer.
     * **/
    struct DrawBatch {
        int num_vertices = 0;
        int start_index;
        int texture_id = -1;

        DrawBatch() = default;

        DrawBatch(int start_index)
            : start_index(start_index) {}

        int size() const {
            return num_vertices;
        }

        int begin() const {
            return start_index;
        }

    };
}
