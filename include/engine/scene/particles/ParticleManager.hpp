#pragma once
#include <vector>

#include "../layers/LayerContext.hpp"
#include "engine/scene/particles/ParticleGroup.hpp"
#include "engine/utilities/SparseVector.hpp"

namespace geng {
    // Manages all the particles in the scene
    class ParticleManager {
    private:
        gutils::SparseVector<geng::ParticleGroup> groups;
        std::vector<int> groups_removed;

        /// Holds the layer context needded for dts
        LayerContext& scene;
    public:
        // Construct/destruct
        explicit ParticleManager(LayerContext& layer_context);
        ~ParticleManager();

        // Updates all particle groups
        void update();

        ParticleGroup *find_by_object(const Gear *o);

        // Adds a particle to the group
        ParticleGroup*& add(ParticleGroup* g);

        void add(const std::vector<ParticleGroup *> &new_groups);

        // Lets a particle die out by ending
        static void dissolve(ParticleGroup* g);
        void dissolve(const Gear *o); //Removes particle groups for any gear

        // Removes a particle from the sparse vector
        void remove(ParticleGroup* g);

        // Checks if there's any particles to be removed
        bool particles_to_remove();
        std::vector<int> pop_removed_particles();
    };
}
