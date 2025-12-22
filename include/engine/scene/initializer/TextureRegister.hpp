#pragma once
#include <iostream>
#include <SDL_render.h>
#include <string>
#include <unordered_map>

#include "Texture.hpp"
#include "engine/layers/LayerContext.hpp"
#include "engine/utilities/image-info/IMGDecoder.hpp"

namespace geng {
    /** @brief The TextureRegister links asset paths to GPU textures via two unordered maps. Each unique file path is assigned an ID, and each ID links to one texture.
     * @details The TextureRegister should know of all textures before main starts. There exists no "post-load asset" feature yet.
     * - To use the TextureRegister, call @code register_texture(string path)@endcode, and it
     * will return an integer with the ID of that texture.
     * - Then the engine should automatically call the @code initialize_textures@endcode method during main, loading all textures into memory.
     * @warning You should not access the texture register yourself. Instead, use the FrameTableRegistry to register FrameTables, which will
     * automatically be assigned a texture ID.
     */
    class TextureRegister {
    private:
        int id_num = 0;
        /// Contains the context for the layer.
        LayerContext& scene;
    public:
        std::unordered_map<std::string, int> path_to_textureID;
        std::unordered_map<int, Texture> ID_to_texture;
    public:
        // Constructs
        explicit TextureRegister(LayerContext& scene);
        ~TextureRegister();

        std::pair<int, Texture> front();

        // Registers a sprite. Returns sheet_id
        int register_texture(std::string path);
        // Initializes all the textures with the renderer. This is essential
        void accept_textures(std::unordered_map<int, Texture> textures);
        // Destroys the texture with the given ID if not needed
        void destroy_texture(int index);

        // Returns true is the texture already exists
        bool has_texture(const std::string &path);
        // Gets the id of a specific path
        int get_id(const std::string &path);

        // Grabs another texture register, inherits it, and initializes it's uninitialized textures
        void join(TextureRegister &reg);
        // Hollows out a TextureRegister. Does not destroy associated textures.
        void clear();

        // Accesses the texture with a specific sheet_id (both do the same thing)
        Texture operator[](const int& index);
        Texture get_texture(int index);

        // Gets the size of the register (number of paths)
        int size();
    };

    TextureRegister& get_tex_register();

}
