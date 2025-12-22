#include "engine/scene/initializer/TextureRegister.hpp"
#include <iostream>

#include "engine/utilities/image-info/IMGDecoder.hpp"

using namespace geng;

TextureRegister::TextureRegister(LayerContext& scene) : scene(scene) {
    std::cerr << "before TextureRegister::TextureRegister()\n";
    scene.log(0, "Making a texture register", "TexutreRegister::TextureRegister");
    id_num = 0;
}

int TextureRegister::register_texture(std::string path) {
    if (path_to_textureID.find(path) == path_to_textureID.end()) {
        int i = id_num++;
        std::cerr << "Registering Path " << path << " Under ID " << i << "\n";
        path_to_textureID[path] = i;
        return i;
    }
    return path_to_textureID[path];
}

void TextureRegister::accept_textures(std::unordered_map<int, geng::Texture> textures) {
    // Goes through all the uninitialized assets and renders them.
    for (auto& [id, tex] : textures) {
        bool pass = false;
        // iterate through and check to make sure the ID really has been registered
        for (auto& [str, sid]: path_to_textureID) {
            if (sid == id) {
                pass = true;
                break;
            }
        }
        if (!pass) {
            std::cerr << "ERR: Texture ID Mismatch for ID " << id << ". No path match\n";
            abort();
        }
        // If the ID already exists, we failed someplace earlier and need to go fix
        if (ID_to_texture.find(id) != ID_to_texture.end()) {
            std::cerr << "ERR: ID " << id << " already registered. [FATAL]\n";
            abort();
        }
        // If all is good we add it to the map.
        ID_to_texture.emplace(id, tex);
    }
}

void TextureRegister::destroy_texture(int index) {
    for (auto& [str, id]: path_to_textureID) {
        if (id == index) {
            path_to_textureID.erase(str);
            break;
        }
    }
    if (ID_to_texture.find(index) != ID_to_texture.end()) {
        SDL_DestroyTexture(ID_to_texture.at(index).texture);
        ID_to_texture.erase(index);
    }

}

bool TextureRegister::has_texture(const std::string &path) {
    if (path_to_textureID.find(path) == path_to_textureID.end()) {
        return false;
    }
    return true;
}

int TextureRegister::get_id(const std::string &path) {
    if (path_to_textureID.find(path) == path_to_textureID.end()) {
        std::cerr << "ERR: Texture not loaded yet.\n";
        return -1;
    }
    std::cerr << "returning from get_id with path " << path << " id: " << path_to_textureID.at(path) << "\n";
    return path_to_textureID[path];
}

void TextureRegister::join(TextureRegister &reg) {
    for (auto& [str, id] : reg.path_to_textureID) {
        if (path_to_textureID.find(str) == path_to_textureID.end()) {
            path_to_textureID[str] = id;
        }
    }
}

void TextureRegister::clear() {
    path_to_textureID.clear();
    ID_to_texture.clear();
}

Texture TextureRegister::get_texture(int index) {
    if (ID_to_texture.find(index) == ID_to_texture.end())
        scene.log(debug::ERROR, "Texture ID " + std::to_string(index) + " does not exist.", "TextureRegister::get_texture");
    return ID_to_texture.at(index);
}

int TextureRegister::size() {
    return path_to_textureID.size();
}

Texture TextureRegister::operator[](const int& index) {
    if (ID_to_texture.find(index) == ID_to_texture.end()) {
        scene.log(debug::ERROR,
            "Texture ID " + std::to_string(index) + " does not exist.",
            "TextureRegister::get_texture");
    }
    return ID_to_texture.at(index);
}

TextureRegister::~TextureRegister() {
    for (auto& [i, tex] : ID_to_texture) {
        SDL_DestroyTexture(tex.texture);
    }
}

std::pair<int, Texture> TextureRegister::front() {
    return {ID_to_texture.begin()->first, ID_to_texture.begin()->second
};
}