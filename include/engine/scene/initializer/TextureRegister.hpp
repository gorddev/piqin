#pragma once

#include <unordered_map>
#include "Texture.hpp"
#include "engine/layers/LayerContext.hpp"
#include "engine/types/strings/hstring/hstring.hpp"

namespace gan {
    /** @brief The TextureRegister links asset paths to GPU textures via two unordered maps. Each unique file path is assigned an ID, and each ID links to one texture.
     * @details The TextureRegister should know of all textures before main starts. There exists no "post-load asset" feature yet.
     * - To use the TextureRegister, call @code register_texture(string path)@endcode, and it
     * will return an integer with the ID of that texture.
     * - Then the engine should automatically call the @code initialize_textures@endcode method during main, loading all textures into memory.
     * @warning You should not access the texture register yourself. Instead, use the FrameTableRegistry to register FrameTables, which will
     * automatically be assigned a texture ID.
     */
    class TextureRegister final {
    private:
        /// Contains the id for each texture.
        int id_num = 0;
        /// Contains the core for the layer.
        EngineContext& core;
        /// Maps the path of the texture to the texture id.
        std::unordered_map<gan::hstring, int> path_to_id;
        /// Maps the id of the texture to the texture object.
        std::unordered_map<int, Texture> id_to_tex;
    protected:
        /// If a texture has been added
        bool dirty = false;
        /// Lets the renderer load a texture
        void load_texture(int index, Texture tex);
        /// Lets the renderer iterate through uninitialized textures
        std::unordered_map<gan::hstring, int> ready_textures;
        /// Clears out any textures waiting to be rendered.
        void clear();
        /// Heyo its me your friend the renderer
        friend class Renderer;
    protected:
        // Constructs
        explicit TextureRegister(EngineContext& core);
        ~TextureRegister();
        friend class Engine;
    public:
        // Gets the front of the texture register
        std::pair<int, Texture> front();

        // Registers a sprites. Returns sheet_id
        int register_texture(hstring path);

        // Adds in a fully created texture
        int instantiate_texture(Texture texture);

        // Destroys the texture with the given ID if not needed
        void unload_texture(int index);
        // Destroys the texture with the given path
        void unload_texture(const char path[]);

        // Returns true is the texture either is ready to be rendered or ready.
        bool has_texture(const char path[]);
        // Returns true if the texture is loaded
        bool is_loaded(const char path[]);
        // Returns true if the texture is loaded
        bool is_loaded(int index);

        // Gets the id of a specific path
        int get_id(const char path[]);

        // Accesses the texture with a specific sheet_id (both do the same thing)
        Texture operator[](int index);
        Texture get_texture(int index);
        Texture get_texture(const char path[]);

        // Gets the size of the register (number of paths)
        int size() const;
    };

    TextureRegister& get_tex_register();
}
