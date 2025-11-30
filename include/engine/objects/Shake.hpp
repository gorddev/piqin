#pragma once
#include "../utilities/types/Vertex.hpp"

namespace gengine {
    // Shake type macros
    enum class GENG_Shake : uint8_t {
        RANDOM = 0,
        CIRCULAR = 1,
        FLOATY = 2,
        SIDE = 3
    };

    // Lets us shake an object's position. (like rumble)
    class Shake {
    private:
        // The current displacement of the object from center
        Vertex displacement;
        // Strength of the shake (total pixel displacement)
        float strength;
        // Speed of the shake
        float speed;
        // Type of shake (random pos, circular, "floaty")
        GENG_Shake shakeType;
        // Whether you want the amplitude of displacement to decay
        bool decay;
        // Tells us when the shake has reached it's end.
        bool complete;
        // Tells us if we do not stop
        bool stop;
        // Duration of the shake. -1 specifies no end
        float duration;
        // Initial duration of the shake
        float initDuration;

        void shake_random();
        void shake_circular();
        void shake_floaty();
        void shake_side();
    public:
        // Constructor for the shaking!
        Shake(GENG_Shake shakeType, float strength, float duration, float speed = 1.0, bool decay = false);

        // Lets us shake the object. Returns true if we're done shaking
        Vertex shake_it();
        // Tells us if we're done shaking
        bool done() const;
        // Ends the shake
        void end();
        // Tells us whether a shake is permanent or not
        bool infinite();
        // Gets the displacement
        Vertex& get_displacement();

    };
}

