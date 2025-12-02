#include "engine/particles/ParticleManager.hpp"

#include <iostream>

using namespace gengine;

ParticleManager::ParticleManager() {
    groups.reserve(40);
}

// SparseVectors don't delete themselves.
ParticleManager::~ParticleManager() {
    for (auto&i: groups) {
        delete i;
    }
}

void ParticleManager::update() {
    for (auto& i : groups) {
        if (i != nullptr) {
            if (i->update()) {
                remove(i);
            }
        }
    }
}

ParticleGroup* ParticleManager::find_by_object(const Object* o) {
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

void ParticleManager::add(std::vector<ParticleGroup*>& groups) {
    for (auto&i: groups)
        add(i);
}

void ParticleManager::dissolve(ParticleGroup *g) {
    g->end();
}

void ParticleManager::dissolve(const Object *o) {
    if (o == nullptr) return;
    for (auto&i: groups) {
        if (i->horse == o) {
            i->end();
        }
    }
}

void ParticleManager::remove(ParticleGroup *g) {
    delete g;
    groups_removed.push_back(g);
    groups.erase(g);
}

bool ParticleManager::particles_to_remove() {
    return !groups_removed.empty();
}

std::vector<int> ParticleManager::pop_removed_particles() {
    std::vector<int> removed_particle_ids(groups_removed.size());
    for (int i = 0; i < groups_removed.size(); i++)
        removed_particle_ids[i] = groups_removed[i]->id;
    groups_removed.clear();
    return removed_particle_ids;
}




