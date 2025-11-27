#pragma once
#include "Vertex.hpp"

// Path Types
#define PATH_LINEAR 0
#define PATH_BALLOON 1
#define PATH_TORPEDO 2
// For path completion with the completion variable
#define PATH_COMPLETE_X 1
#define PATH_COMPLETE_Y 2
#define PATH_COMPLETE_Z 4
#define PATH_COMPLETE 7
// Determines various constants for the paths
#define PATH_BALLOON_DAMP 50.0f
#define PATH_TORPEDO_AMPLIFY 100.0f
class Path {
private:
    Vertex target;
    float initDist;
    float speed;
    uint8_t pathType;
    uint8_t complete = 0;

    // Types of path functions
    // Scales linearly to target
    void linear(Vertex& pos);
    // Moves fast, then slows as approaches
    void balloon(Vertex& pos);
    // Moves slow, and speeds up on approach.
    void torpedo(Vertex& pos);
    // Helper functions
    // Gets the sign difference between pos and target
    // e.g. pos.x > target.x, xm = -1;

    // Tells us if we overshoot given our distance
    bool overshoot(double& o, double& t, double dist) const;

public:
    // Constructor for a path.
    Path(const Vertex &target, const Vertex &startPos, uint8_t pathType, float speed);
    // Updates the vertex according to the pathType.
    void to_path(Vertex& pos);
    // Returns true if a path is complete
    bool path_complete() const;
    // Returns the target
    Vertex get_target();
    // Returns a string
    std::string to_string();

};