#pragma once

#include "Vertex.hpp"
// Shake type macros
#define SHAKE_RANDOM 0
#define SHAKE_CIRCULAR 1
#define SHAKE_FLOATY 2
#define SHAKE_SIDE 3
// Macros for speed stuff
#define SHAKE_CIRCULAR_SPEED 0.001f
#define SHAkE_FLOATY_SPEED 0.005f
// Lets us shake an object's position. (like rumble)
class Shake {
private:
    // The current displacement of the object from center
    Vertex displacement;
    // Gives us a position to pop back to if so desired.
    Vertex popPos;
    // Strength of the shake (total pixel displacement)
    float strength;
    // Speed of the shake
    float speed;
    // Type of shake (random pos, circular, "floaty")
    uint8_t shakeType;
    // Whether you want the amplitude of displacement to decay
    bool decay;
    // Tells us when the shake has reached it's end.
    bool complete;
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
    Shake(uint8_t shakeType, float strength, int duration, float speed = 1.0, bool decay = false);

    // Lets us shake the object. Returns true if we're done shaking
    bool shake_it(Vertex& pos);
    // Tells us if we're done shaking
    bool done();
    // Ends the shake
    void end();

    // Sets position pop if we care about shaking about a point.
    void set_pos_pop(Vertex& v);
    Vertex pop_pos();
};
