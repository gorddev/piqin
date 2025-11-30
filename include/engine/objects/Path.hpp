#pragma once
#include "../utilities/types/Vertex.hpp"

namespace gengine {

    // Types of paths
    enum class GENG_Path : uint8_t {
        LINEAR = 0,
        BALLOON = 1,
        TORPEDO = 2,
        SINE = 3
    };

    // Scaling of paths
    enum class PathScaling : short {
        BALLOON = 50,
        TORPEDO = 100
    };

    class Path {
    private:
        Vertex target;
        Vertex startPos;
        float initDist;
        float speed;
        GENG_Path pathType;
        uint8_t complete = 0;

        // Types of path functions
        // Scales linearly to target
        void linear(Vertex& pos);
        // Moves fast, then slows as approaches
        void balloon(Vertex& pos);
        // Moves slow, and speeds up on approach.
        void torpedo(Vertex& pos);
        // Keeps x linear, does cos w/y
        void sine(Vertex& pos);
        // Helper functions
        // Gets the sign difference between pos and target
        // e.g. pos.x > target.x, xm = -1;

        // Tells us if we overshoot given our distance
        bool overshoot(float& o, float& t, float dist) const;

    public:
        // Constructor for a path.
        Path(const Vertex &target, const Vertex &startPos, GENG_Path pathType, float speed);
        // Updates the vertex according to the pathType.
        void to_path(Vertex& pos);
        // Returns true if a path is complete
        [[nodiscard]] bool path_complete() const;
        // Returns the target
        Vertex get_target();
        // Returns a string
        std::string to_string();
    };
}