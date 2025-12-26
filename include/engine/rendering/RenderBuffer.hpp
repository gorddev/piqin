#pragma once
#include <SDL_render.h>


#include "Camera.hpp"
#include "DrawBatch.hpp"
#include "engine/scene/initializer/TextureRegister.hpp"
#include "engine/types/positioning/FPos2D.hpp"
#include "engine/utilities/image-info/IMGDecoder.hpp"
#include "shadows/ShadowBank.hpp"

namespace geng {
    namespace debug {
        class Console;
    }

    /** The RenderBuffer contains all the information needed for the renderer to batch render calls. @code Layers@endcode append
     * vertices to the render buffer, and the RenderBuffer offsets by camera effects. The RenderBuffer also has the capacity to handle
     * undefined textures and the like*/
    class RenderBuffer final {
        /// The buffer that contains all the vertices we will render
        gch::vector<SDL_Vertex> buffer;
        /// The buffer of batches if the renderer changes textures
        TextureRegister& texreg;
        /// Contains the current camera position of the scene
        FPos2D campos;
        /// Keeps track of the bank of shadows
        ShadowBank& shadows;
        /// Contains the white point of the current texture we're rendering.
        SDL_FPoint white_point = {1.f, 1.f};
        /// If our texture is not loaded:
        bool loaded = true;
        /// If we're in debug mode
        bool debug = false;
        /// Preps a vertex to be added
        void prep_vertex(SDL_Vertex& vertex);
    protected:
        /// Contains each of the batches we'll render
        gch::vector<DrawBatch> batches;
        /// Contains the current draw batch
        DrawBatch current_batch;

        friend class Renderer; // hey look a friend

        /// Renders console regardless of camera pos
        void debug_mode(bool mode);
        friend class debug::Console;

        /// Returns the data of the buffer
        [[nodiscard]] SDL_Vertex* data();
        /// Size of the buffer
        [[nodiscard]] int size() const;
        /// Sets the position of the camera
        void prep(FPos2D camera_pos);
        /// Resize the render buffer
        void resize(int num);
        /// Pops the last batch onto the batch vector
        void pop_last_batch();
    public:
        /// Constructed render buffer
        explicit RenderBuffer(TextureRegister& texreg, ShadowBank& shadows);

        /// Changes the current texture
        void request_texture(int id);

        // <><><> White Point <><><>
        /// Gets the white point of the current texture
        SDL_FPoint get_white_point();

        // <><><> Standard push back functions <><><>
        /// Adds vertex to the render buffer
        void push_back(SDL_Vertex vertex);
        /// Adds the properties of a FPos2D to the RenderBuffer to be added to the FPos2D buffer
        void push_back(SDL_FPoint& pos, SDL_Color& color, SDL_FPoint& tex_coord);
        /// Adds a buffer to the current buffer
        void push_back(gch::vector<SDL_Vertex>& vertices);

        // <><><> Point-based push back functions
        /// Adds a point to the buffer -- thus ensuring that it is a single color. Default color is white.
        void push_back(SDL_FPoint pos, SDL_Color color = {255, 255, 255, 255});
        /// Adds many points to the buffer with the specified color
        void push_back(gch::vector<SDL_FPoint>& pos, SDL_Color color = {255, 255, 255, 255});

        //<><><> Shadow Handling <><><>
        /// Gets the reference to the shadow bank
        ShadowBank& get_shadow_bank();
        /// Renders a certain number of shadows from the top of the FPos2D buffer with the default shadow type
        void push_shadow(int numShadows);
        /// Renders one shadow to the most recently added 3 Vertexes.
        void push_shadow();
        /// Adds shadows to a specific supplied buffer, which is then added to the regular buffer
        void instanced_push_shadow(gch::vector<SDL_Vertex>& vertices, int numShadows);

    };

} // namespace geng