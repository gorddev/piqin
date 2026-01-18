#pragma once
#include "ParticleState.hpp"
#include "ParticleTypes.hpp"

namespace gan {
    class LayerState;

    /* Used to systematically generate particles of all kinds. Has several key parameters: */
    class ParticleGenerator {
    private:
        /// Holds the data for a ParticleState
        const ParticleState state;
        /// Ties this generator to a set of particle functions
        gch::vector<ParticleFunction> particle_functions;
        /// Ties this generator to a set of particle primers to create particles
        gch::vector<ParticlePrimer> particle_primers;
        /// Ties this generator to a specific group id
        int group_id = -1;
        /// Ties this generator to a specific ShapeType
        int shape_type = -1;
        /// The duration of the ParticleGenerator.
        int duration = -1;
        /// Whether the ParticleGenerator is permanent or not
        bool permanent = true;
        /// Lets the particle manager interact with relevant data
        friend class ParticleManager;
    public:
        /// Constructs a particleGenerator
        ParticleGenerator(ParticleState state, int group_id, int shape_type);

        /// Updates a particle manager. Returns the number of particles to be generated on any given frame
        uint16_t update(LayerState& lstate) {
            state.
        }
    };
}
