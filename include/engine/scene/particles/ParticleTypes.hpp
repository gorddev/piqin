#pragma once

#include "engine/types/positioning/vec2.hpp"

namespace gan {
    struct Particle;
    struct ParticleState;

    /// @brief A @code ParticleType@endcode is a fixed array that contains all the unit vector points necessary to generate a particle.
    /// Each ParticleType has a predefined array of unit vertices used for reference, which is stored on the stack.
    /// @details ParticleTypes should not have information about size or duration. Instead, they simply represent the shape of the particle.
    /// For example, a rhombus would be represented by a ParticleType of size 4 with the unit vectors: @code {0, -1}, {-1, 0}, {0, 1}, {0, 1}@endcode
    using ParticleShape = const std::vector<FPos2D>;

    /// @brief A @code ParticleFunction@endcode takes in the information from a @code ParticleType@endcode and modifies the vertices.
    /// @details For example, say you want to decrease the size of a particle with time. The @code ParticleFunction@endcode takes in a @code Particle&@endcode
    /// and a @code ParticleState&@endcode and will decrease the size comparing the particle's duration with the base duration:
    /// - @code bool changeSize(Particle& p, ParticleState& state) { p.size = p.duration/state.baseDuration; return p.size <= 0; }@endcode
    /// - returning @code true@endcode indicates that the particle is done and should be removed from the particle queue.
    /// @note ParticleFunctions will be called every frame.
    using ParticleUpdater = bool (Particle& p, ParticleState& state);

    /// @brief A @code ParticlePrimer@endcode is a function used to set the base stats of each particle upon creation.
    /// @details. This function acts as the "creator of particles" and will continue to produce particles as long as the parent
    /// ParticleState remains alive. For example, if I wanted each particle to start off with a random velocity -1 or 1, I would write:
    /// - @code Particle myFunc(ParticleState& state) {
    ///     return Particle(state.baseDuration, {random()%2 * (random % 2 ? 1 : -1)});
    /// }@endcode
    /// @note ParticlePrimers are called once upon creation of each particle.
    using ParticlePrimer = void (Particle& p, ParticleState& state);
}