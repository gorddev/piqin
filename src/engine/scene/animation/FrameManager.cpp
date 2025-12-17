#include <iostream>
#include "engine/scene/animation/FrameManager.hpp"


using namespace geng;


void FrameManager::add_tables(const std::vector<FrameTable>& frameTables) {
    // Links each Frame Table to a Table ID.
    for (auto& i: frameTables) {
        tables.emplace_back(i);
    }
}

void FrameManager::apply_framestate(Actor &a) {
    a.anim.set_frame(tables[a.anim.get_frame_table_id()].get_frame(a.anim.get_anim_id()));
}

void FrameManager::apply_framestates(const std::vector<Actor *> &actors) {
    for (auto &a : actors) {
        apply_framestate(*a);
    }
}


int FrameManager::get_texture_id(Mesh& m) {
    return m.texture_id;
}

FrameTable& FrameManager::get_table(Actor &a) {
    return tables[a.anim.get_frame_table_id()];
}

void FrameManager::update(const std::vector<AnimInfo*>& anims) {
    for (auto& anim : anims)
        tables[anim->get_frame_table_id()].update_frame(*anim);
}

