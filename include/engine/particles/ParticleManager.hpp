#pragma once
#include <vector>
#include "engine/particles/ParticleGroup.hpp"
#include "../utilities/types/SparseVector.hpp"

namespace gengine {
    // Manages all the particles in the scene
    class ParticleManager {
    private:
        utils::SparseVector<ParticleGroup> groups;

    public:
        // Construct/destruct
        ParticleManager();
        ~ParticleManager();

        // Updates all particle groups
        void update();

        // Adds a particle to the group
        ParticleGroup*& add(ParticleGroup* g);

        // Removes a particle from the group
        void dissolve(ParticleGroup* g);
    };
}
