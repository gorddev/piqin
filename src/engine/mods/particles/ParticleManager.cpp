#include "engine/scene/particles/ParticleManager.hpp"

#include "../../../../include/engine/layers/layer-subobjects/LayerCore.hpp"

using namespace gan;

ParticleManager::ParticleManager(LayerCore& layer_context): scene(layer_context) {
    groups.reserve(40);
}

// SparseVectors don't delete themselves.
ParticleManager::~ParticleManager() {
    for (auto&i: groups) {
        delete i;
    }
}

void ParticleManager::remove(ParticleGroup *g) {
    DONOTUSE.push_back(g->id);
    groups_to_remove.push_back(g);
    groups.erase(g);
    delete g;
}


std::vector<ParticleGroup*> ParticleManager::pop_removed_particles() {
    std::vector<ParticleGroup*> removed_particles = groups_to_remove;
    groups_to_remove.clear();
    return removed_particles;
}

gutils::SparseVector<ParticleGroup> &ParticleManager::active_particles() {
    return groups;
}

void ParticleManager::update() {
    size_t sz = groups.size();
    for (size_t i = 0; i < sz; ++i) {
        ParticleGroup* g = groups[i];
        if (!g) continue;

        if (g->update(scene.state)) {
            DONOTUSE.push_back(g->id);
            groups_to_remove.push_back(g);
            groups.erase(g);
            delete g;
        }
    }
}

ParticleGroup* ParticleManager::find_by_object(const Gear* o) {
    for (auto& i: groups) {
        if (i != nullptr && i->payload == o) {
            return i;
        }
    }
    return nullptr;
}

ParticleGroup*& ParticleManager::add(ParticleGroup *g) {
    return groups[groups.add(g)];
}

void ParticleManager::add(std::vector<ParticleGroup*>& new_groups) {
    for (auto&i: new_groups)
        add(i);
}

void ParticleManager::dissolve(ParticleGroup *g) {
    g->end();
}

void ParticleManager::dissolve(const Gear *o) {
    if (o == nullptr) return;
    for (auto&i: groups) {
        if (i != nullptr && i->payload == o) {
            i->end();
        }
    }
}

bool ParticleManager::particles_to_remove() {
    return !groups_to_remove.empty();
}



