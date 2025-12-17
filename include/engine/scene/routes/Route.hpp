#pragma once
#include "engine/types/Transform.hpp"
#include "engine/types/Gear.hpp"
#include "engine/scene/layers/LayerTime.hpp"

namespace geng {

    /** @brief Path is an abstract base class that moves transforms objects across the canvas.
     * @details Paths update a @code gengine::Transform@endcode object until they reach their @code destination@endcode.
     * All Path objects must override the function @code void update()@endcode function. Here is a quick rundown of member variables:
     * 1. @code Transform* payload@endcode › The transform object you update with the route
     * 2. @code Vertex destination@endcode › The destination of the route
     * 3. @code Vertex start@endcode › the starting point of the route
     * 4. @code float speed@endcode › Speed of movement along the route.
     * 5. @code bool completeX, completeY, completeZ@endcode › Helper variables to specify when a route is complete in each axis
     * 6. @code function<void()>* callback@endcode › Calls this function on route completion. nullptr by default.
     * - @code bool overshoot(float& source, float& target, float dist)@endcode ›› Returns true if moving a distance from source to target shoots past the target.
     * WARNING: Will update the actual floats provided if you don't make copies.
     * - @code Vertex get_destination()@endcode ›› gets the destination of the route.
     * - @code int get_trasnform_id()@endcode ›› gets the payload's id.
     * - @code std::string to_string()@endcode ›› prints out the route (overridable)
     */
    class Route {
    protected:
        Gear* gear;
        Vertex target;
        Vertex start;
        float speed;
        bool completeX = false;
        bool completeY = false;
        bool completeZ = false;
        /// Calls this function when the Route is complete
        std::function<void()>* callback = nullptr;

        /// Tells us if we overshoot given our distance
        static bool overshoot(float& source, const float& target, float dist) {
            const float newpos = source + dist;
            if ((dist > 0 && newpos > target) || (dist < 0 && newpos < target)) {
                source = target;
                return true;
            }
            source = newpos;
            return false;
        }

    public:
        /// Constructor for a route.
        Route(Gear* gear, const Vertex &destination, float speed)
            : gear(gear), start(gear->t.pos), target(destination), speed(speed) {}
        virtual ~Route() = default;

        /// Updates the gear according to the subclass' definition
        virtual bool update(LayerTime& time) = 0;
        
        /// Returns the target vertex
        [[nodiscard]] Vertex get_target() const { return target; }
        /// Returns the gear
        [[nodiscard]] Gear* get_payload() const { return gear; }

        /// Updates the target of the route
        void set_target(Vertex v) { target = v; start = gear->t.pos; }
        /// Sets the callback function, which is called upon route completion
        void set_callback(std::function<void()>* cb) { callback = cb; }
        /// Returns a string with route information.
        virtual std::string to_string() {
            std::string ret = "Target: "  + target.to_string();
            ret += "\nSpeed: " + std::to_string(speed);
            ret += "\nComplete: " + std::to_string(int(completeX));
            return ret;
        }
    };
}
