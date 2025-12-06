#include "engine/textures/SheetManager.hpp"

#include <iostream>

#include "../../../include/engine/textures/asset-info/TextureRegister.hpp"
#include "engine/gengine-globals/scene.hpp"
#include "engine/textures/asset-info/SheetRegistry.hpp"

using namespace gengine;

SheetManager::SheetManager() { }

void SheetManager::initialize(SDL_Renderer* renderer) {
    textures::tex_register.initialize_textures(renderer);
    sheets.resize(textures::getRegistry().size());
    for (auto& [id, sheet] : textures::getRegistry()) {
        sheets[id] = sheet;
    }
}

void SheetManager::apply_framestate(Object &o) {
    o.fs.set_frame(sheets[o.fs.frame_sheet_id].get_frame(o.fs.animation_index));
}

void SheetManager::apply_framestates(
    std::vector<Object *> objs) {
    for (auto &i : objs) {
        apply_framestate(*i);
    }
}


int SheetManager::get_texture_id(Mesh& m) {
    return m.texture_id;
}

textures::FrameSheet& SheetManager::get_sheet(Object &o) {
    return sheets[o.fs.frame_sheet_id];
}

void SheetManager::update(std::vector<FrameState*> states) {
    for (auto& spr : states) {
        std::cerr << "\ncustomer: " << spr->to_string() << std::endl;
        sheets[spr->frame_sheet_id].update_frame(*spr);
        std::cerr << "exiting: " << spr->to_string() << std::endl;
    }
}
