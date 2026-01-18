#pragma once
#include <cstdint>

namespace gan {
    struct ParticleState {
        /** Current particle that has been generated **/
        uint64_t particle_num = 0;
        /** Time since the creation of the particle group **/
        uint64_t time = 0;
        /** Base duration of each particle within the particle group **/
        float base_duration = -1.f;
        /** Base size of each particle within the particle group **/
        float base_size = 1.f;
        /** Base speed of each particle within the particle group **/
        float base_speed = 1.f;
        /** Base "strength" of each particle within the particle group. "Strength" is left up to user interpretation*/
        float base_strength = 0.f;
        /** Period of Particle Generation **/
        float period;
    };
}