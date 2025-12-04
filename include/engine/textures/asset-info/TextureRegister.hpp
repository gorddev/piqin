#pragma once
#include <SDL_render.h>
#include <string>
#include <unordered_map>

namespace gengine::textures {
    // Keeps track of how IDs relate to each texture
    // In the best case scenario, you have one texture that all ids map to.
    class TextureRegister {
    private:
        std::unordered_map<std::string, int> path_to_textureID;
        std::unordered_map<int, SDL_Texture*> ID_to_texture;
    public:
        // Constructs
        TextureRegister();
        ~TextureRegister();

        // Registers a sprite. Returns sheet_id
        int register_texture(std::string path);
        // Initializes all the textures with the renderer. This is essential
        void initialize_textures(SDL_Renderer* renderer);
        // Destroys the texture with the given ID if not needed
        void destroy_texture(int index);

        // Accesses the texture with a specific sheet_id (both do the same thing)
        SDL_Texture* operator[](const int& index);
        SDL_Texture* get_texture(int index);
    };

    inline TextureRegister tex_register;
}