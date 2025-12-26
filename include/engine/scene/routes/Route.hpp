#pragma once
#include "engine/types/Transform2D.hpp"
#include "../../core/gears/Gear.hpp"
#include "engine/layers/LayerState.hpp"

namespace geng {

    /** @brief Path is an abstract base class that moves transforms objects across the canvas.
     * @details Paths update a @code gengine::Transform2D@endcode object until they reach their @code destination@endcode.
     * All Path objects must override the function @code void update()@endcode function. Here is a quick rundown of member variables:
     * 1. @code Transform2D* payload@endcode › The transform object you update with the route
     * 2. @code FPos2D destination@endcode › The destination of the route
     * 3. @code FPos2D start@endcode › the starting point of the route
     * 4. @code float speed@endcode › Speed of movement along the route.
     * 5. @code bool completeX, completeY, completeZ@endcode › Helper variables to specify when a route is complete in each axis
     * 6. @code function<void()>* callback@endcode › Calls this function on route completion. nullptr by default.
     * - @code bool overshoot(float& source, float& target, float dist)@endcode ›› Returns true if moving a distance from source to target shoots past the target.
     * WARNING: Will update the actual floats provided if you don't make copies.
     * - @code FPos2D get_destination()@endcode ›› gets the destination of the route.
     * - @code int get_trasnform_id()@endcode ›› gets the payload's id.
     * - @code std::string to_string()@endcode ›› prints out the route (overridable)
     */
    class Route {
    protected:
        Gear& gear;
        FPos2D target;
        FPos2D start;
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
        Route(Gear& gear, const FPos2D &destination, float speed)
            : gear(gear), start(gear.t.pos), target(destination), speed(speed) {}
        virtual ~Route() = default;

        /// Updates the gear according to the subclass' definition
        virtual bool update(LayerState& time) = 0;
        
        /// Returns the target FPos2D
        [[nodiscard]] FPos2D get_target() const { return target; }
        /// Returns the gear
        [[nodiscard]] Gear* get_payload() const { return &gear; }

        /// Updates the target of the route
        void set_target(FPos2D v) { target = v; start = gear.t.pos; }
        /// Sets the callback function, which is called upon route completion
        void set_callback(std::function<void()>* cb) { callback = cb; }
        /// Returns a string with route information.
        virtual geng::str_view to_string(geng::str_view& ret) {
            ret << "Target: "  << target.to_string(ret)
                << "\nSpeed: " << speed
                << "\nComplete: " << int(completeX);
            return ret;
        }
    };
}
