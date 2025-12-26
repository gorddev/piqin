#include "engine/scene/initializer/TextureRegister.hpp"

#include "../../../../include/engine/types/strings/fstring/fstring.hpp"
#include "../../../../include/engine/debug/geng_debug.hpp"
#include "engine/utilities/image-info/IMGDecoder.hpp"

using namespace geng;

TextureRegister::TextureRegister(EngineContext& core) : core(core) {
    id_num = 0;
}

int TextureRegister::register_texture(hstring path) {
    // Checks if the texture is added yet
    if (path_to_id.find(path) == path_to_id.end()) {
        // Set dirty to true
        dirty = true;
        // Incremnet id by one
        int i = id_num++;
        // Adds a path linked to texture id
        path_to_id[path] = i;
        // Adds a ready texture
        ready_textures[path] = i;
        // Returns the number
        return i;
    }
    // Just returns the texture id.
    return path_to_id[path];
}

void TextureRegister::load_texture(int index, Texture tex) {
    glog::note << "Loading Texture: " << tex.info.filename << " (" << tex.texture << ')' << glog::endlog;
    id_to_tex[index] = tex;
}
void TextureRegister::clear() {
    ready_textures.clear();
}

void TextureRegister::unload_texture(const char* name) {
    if (path_to_id.find(name) != path_to_id.end())
        unload_texture(path_to_id.at(name));
    else {
        glog::warn.src("TextureRegister::unload_texture")
            << "Texture " << name << " does not exist." << glog::endlog;
    }
}

void TextureRegister::unload_texture(int index) {
    geng::hstring path;
    for (auto it = path_to_id.begin(); it != path_to_id.end(); ++it) {
        if (it->second == index) {
            path = it->first;
            break;
        }
    }
    if (!path.empty()) {
        auto it = ready_textures.find(path);
        if (it != ready_textures.end())
            ready_textures.erase(it);
    }
    // Erase and destroy the texture in the id map
    auto it2 = id_to_tex.find(index);
    if (it2 != id_to_tex.end()) {
        SDL_DestroyTexture(it2->second.texture);
        id_to_tex.erase(it2);
    }
}

bool TextureRegister::has_texture(const char path[]) {
    // Returns true if it has the texture
    if (path_to_id.find(path) == path_to_id.end()) {
        return false;
    }
    return true;
}

bool TextureRegister::is_loaded(const char path[]) {
    if (path_to_id.find(path) == path_to_id.end())
        return false;
    return (id_to_tex.find(path_to_id.at(path)) != id_to_tex.end());
}

bool TextureRegister::is_loaded(int index) {
    if (id_to_tex.find(index) == id_to_tex.end())
        return false;
    return true;
}

int TextureRegister::get_id(const char path[]) {
    if (path_to_id.find(path) == path_to_id.end()) {
        glog::warn.src("TextureRegister::get_id")
           << "Texture " << path << " not loaded." << glog::endlog;
        return -1;
    }
    return path_to_id[path];
}

Texture TextureRegister::get_texture(const char path[]) {
    if (path_to_id.find(path) == path_to_id.end()) {
        glog::err.src("TextureRegister::get_texture")
            << "Texture " << path << " does not exist." << glog::endlog;
    }
    else if (id_to_tex.find(path_to_id[path]) == id_to_tex.end()){
        glog::err.src("TextureRegister::get_texture")
            << "Texture " << path << " does not exist." << glog::endlog;
    }
    return id_to_tex.at(path_to_id[path]);
}

Texture TextureRegister::get_texture(int index) {
    return (*this)[index];
}

Texture TextureRegister::operator[](int index) {
    if (id_to_tex.find(index) == id_to_tex.end()) {
        glog::err.src("TextureRegister::operator[]")
        << "Texture ID " << index << " does not exist." << glog::endlog;
    }
    return id_to_tex.at(index);
}

int TextureRegister::size() const {
    return static_cast<int>(path_to_id.size());
}

TextureRegister::~TextureRegister() {
    for (auto& [i, tex] : id_to_tex) {
        SDL_DestroyTexture(tex.texture);
    }
}

std::pair<int, Texture> TextureRegister::front() {
    return {id_to_tex.begin()->first, id_to_tex.begin()->second};
}