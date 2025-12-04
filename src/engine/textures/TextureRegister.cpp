#include "engine/textures/asset-info/TextureRegister.hpp"
#include <iostream>
#include "engine/utilities/BasisDecoder.hpp"

using namespace gengine::textures;

TextureRegister::TextureRegister() {}


int TextureRegister::register_texture(std::string path) {
    if (path_to_textureID.find(path) == path_to_textureID.end()) {
        int i = ID_to_texture.size();
        path_to_textureID[path] = i;
        return i;
    }
    return path_to_textureID[path];
}

void TextureRegister::initialize_textures(SDL_Renderer *renderer) {
    // Goes through all the uninitialized assets and renders them.
    for (auto& [path, i] : path_to_textureID)
        if (ID_to_texture.find(i) == ID_to_texture.end())
            ID_to_texture[i] = BasisDecoder::load_ktx2_as_texture(renderer, path);
}

void TextureRegister::destroy_texture(int index) {
    if (ID_to_texture.find(index) != ID_to_texture.end()) {
        SDL_DestroyTexture(ID_to_texture[index]);
        ID_to_texture.erase(index);
    }
}

SDL_Texture* TextureRegister::get_texture(int index) {
    if (ID_to_texture.find(index) == ID_to_texture.end()) {
        std::cerr << "TextureRegister::Texture ID does not exist. [FATAL]\n";
        return nullptr;
    }
    return ID_to_texture[index];
}

SDL_Texture* TextureRegister::operator[](const int& index) {
    if (ID_to_texture.find(index) == ID_to_texture.end()) {
        std::cerr << "TextureRegister::Texture ID does not exist. [FATAL]\n";
        return nullptr;
    }
    return ID_to_texture[index];
}

TextureRegister::~TextureRegister() {
    for (auto& [i, tex] : ID_to_texture) {
        SDL_DestroyTexture(tex);
    }
}
