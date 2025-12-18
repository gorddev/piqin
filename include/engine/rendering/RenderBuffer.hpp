#pragma once
#include <SDL_render.h>
#include <vector>

#include "engine/utilities/image-info/IMGDecoder.hpp"
#include "shadows/ShadowBank.hpp"

namespace geng {

    class RenderBuffer final {
        /// The buffer that contains all the vertices we will render
        std::vector<SDL_Vertex> buffer;
        /// Contains the width and height of the current texture we're rendering
        IMG_Info tex_info;
        /// Contains the white point of the current texture we're rendering.
        SDL_FPoint white_point = {1.f, 1.f};
        /// Keeps track of the bank of shadows
        ShadowBank shadows;
    public:
        /// Constructed render buffer
        explicit RenderBuffer(const ShadowBank& shadows) : buffer(10000),
            tex_info("no image", 0, 0),
            shadows(shadows) {}

        /// Adds vertex to the render buffer
        void push_back(SDL_Vertex vertex) {
            buffer.push_back(vertex);
        }
        /// Adds the properties of a vertex to the RenderBuffer to be added to the vertex buffer
        void push_back(SDL_FPoint& pos, SDL_Color& color, SDL_FPoint& tex_coord) {
            buffer.push_back({pos, color, tex_coord});
        }
        /// Adds a point to the buffer -- thus ensuring that it is a single color. Default color is white.
        void push_back(SDL_FPoint pos, SDL_Color color = {255, 255, 255, 255}) {
            buffer.push_back(
            { pos, color, white_point}
            );
        }
        /// Adds a buffer to the current buffer
        void push_back(std::vector<SDL_Vertex>& vertices) {
            buffer.insert(buffer.end(), vertices.begin(), vertices.end());
        }

        //<><><> Shadow Handling <><><>
        /// Renders a certain number of shadows from the top of the vertex buffer with a shadow_type
        void push_shadow(int numShadows, std::string shadowType) {
            shadows.apply_shadow(buffer, numShadows, std::move(shadowType));
        }
        /// Renders a certain number of shadows from the top of the vertex buffer with the default shadow type
        void push_shadow(int numShadows) {
            shadows.apply_shadow(buffer, numShadows);
        }
        /// Renders one shadow to the most recently added vertex.
        void push_shadow() {
            shadows.apply_shadow(buffer);
        }
        /// Adds shadows to a specific supplied buffer given the specified shadow type
        void instanced_push_shadow(std::vector<SDL_Vertex>& vertices, int numShadows, std::string shadowType) {
            shadows.apply_shadow(vertices, numShadows, std::move(shadowType));
        }
        /// Adds shadows to a specific supplied buffer
        void instanced_push_shadow(std::vector<SDL_Vertex>& vertices, int numShadows) {
            shadows.apply_shadow(vertices, numShadows);
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
