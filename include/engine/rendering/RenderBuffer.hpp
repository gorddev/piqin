#pragma once
#include <SDL_render.h>
#include <vector>

#include "engine/utilities/image-info/IMGDecoder.hpp"

namespace geng {

    class RenderBuffer final {
        std::vector<SDL_Vertex> buffer;
        IMG_Info tex_info;
        SDL_FPoint white_point = {1.f, 1.f};
    public:
        /// Constructed render buffer
        RenderBuffer() : buffer(10000), tex_info("no image", 0, 0) {}

        /// Adds vertex to the render buffer
        void push_back(SDL_Vertex vertex) {
            buffer.push_back(vertex);
        }
        void push_back(SDL_FPoint& pos, SDL_Color& color, SDL_FPoint& tex_coord) {
            buffer.push_back({pos, color, tex_coord});
        }
        void push_back(SDL_FPoint pos, SDL_Color color = {255, 255, 255, 255}) {
            buffer.push_back(
            { pos, color, white_point}
            );
        }
        /// Size of the buffer
        [[nodiscard]] long size() const {
            return buffer.size();
        }

        /// Returns the data of the buffer
        [[nodiscard]] SDL_Vertex* data() {
            return buffer.data();
        }
        /// Clears the buffer
        void clear() {
            buffer.clear();
        }
        /// Sets the image info
        void set_img_info(IMG_Info info) {
            tex_info = info;
            white_point = {(tex_info.w - 0.5f)/tex_info.w,(tex_info.h - 0.5f)/tex_info.h};
        }
        /// Resize the render buffer
        void resize(int num) {
            buffer.resize(num);
        }

    };
}
