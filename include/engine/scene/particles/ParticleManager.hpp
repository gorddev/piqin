#pragma once

#include "engine/layers/LayerContext.hpp"
#include "engine/scene/particles/ParticleGroup.hpp"
#include "engine/utilities/SparseVector.hpp"

namespace gan {
    // Manages all the particles in the scene
    class ParticleManager {
    private:
        gutils::SparseVector<gan::ParticleGroup> groups;
        gch::vector<int> DONOTUSE;

        /// Holds the layer core needded for dts
        LayerContext& scene;
        /// Keeps track of the groups to remove
        gch::vector<ParticleGroup*> groups_to_remove;
    public:
        // Construct/destruct
        explicit ParticleManager(LayerContext& layer_context);
        ~ParticleManager();

        // Updates all particle groups
        void update();

        ParticleGroup *find_by_object(const Gear *o);

        // Adds a particle to the group
        ParticleGroup*& add(ParticleGroup* g);

        void add( gch::vector<ParticleGroup *> &new_groups);

        // Lets a particle die out by ending
        static void dissolve(ParticleGroup* g);
        void dissolve(const Gear *o); //Removes particle groups for any gear

        // Removes a particle from the sparse vector
        void remove(ParticleGroup* g);

        // Checks if there's any particles to be removed
        bool particles_to_remove();
        // Pops all the removed particles
        gch::vector<ParticleGroup*> pop_removed_particles();

        // Returns the list of particles
        gutils::SparseVector<ParticleGroup> &active_particles();
    };
}
