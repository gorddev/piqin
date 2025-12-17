#include "engine/scene/particles/ParticleManager.hpp"

#include <iostream>

#include "../../../../include/engine/scene/layers/LayerContext.hpp"

using namespace geng;

ParticleManager::ParticleManager(LayerContext& layer_context): scene(layer_context) {
    groups.reserve(40);
}

// SparseVectors don't delete themselves.
ParticleManager::~ParticleManager() {
    for (auto&i: groups) {
        delete i;
    }
}

void ParticleManager::remove(ParticleGroup *g) {
    groups_removed.push_back(g->id);
    groups.erase(g);
    delete g;
}


std::vector<int> ParticleManager::pop_removed_particles() {
    std::vector<int> removed_particle_ids = groups_removed;
    groups_removed.clear();
    return removed_particle_ids;
}

void ParticleManager::update() {
    size_t size = groups.size();
    // Lets us keep track of the particle groups we need to remove
    std::vector<ParticleGroup*> groups_to_remove;

    for (size_t index = 0; index < size; index++) {
        ParticleGroup* g = groups[index];
        if (g != nullptr) {
            // update returns true if should remove
            if (g->update(scene.time)) {
                groups_removed.push_back(g->id);
                groups_to_remove.push_back(g);
            }
        }
    }
    for (auto g: groups_to_remove) {
        groups.erase(g);
        delete g;
    }
}

ParticleGroup* ParticleManager::find_by_object(const Gear* o) {
    for (auto& i: groups) {
        if (i->horse == o) {
            return i;
        }
    }
    return nullptr;
}

ParticleGroup*& ParticleManager::add(ParticleGroup *g) {
    return groups[groups.add(g)];
}

void ParticleManager::add(const std::vector<ParticleGroup*>& new_groups) {
    for (auto&i: new_groups)
        add(i);
}

void ParticleManager::dissolve(ParticleGroup *g) {
    g->end();
}

void ParticleManager::dissolve(const Gear *o) {
    if (o == nullptr) return;
    for (auto&i: groups) {
        if (i->horse == o) {
            i->end();
        }
    }
}

bool ParticleManager::particles_to_remove() {
    return !groups_removed.empty();
}



