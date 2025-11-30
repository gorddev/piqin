#include "engine/particles/ParticleManager.hpp"

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
    for (auto& i : groups)
        i->update();
}

ParticleGroup*& ParticleManager::add(ParticleGroup *g) {
    return groups[groups.add(g)];
}

void ParticleManager::add(std::vector<ParticleGroup*>& groups) {
    for (auto&i: groups)
        add(i);
}

void ParticleManager::dissolve(ParticleGroup *g) {
    groups.erase(g);
}




