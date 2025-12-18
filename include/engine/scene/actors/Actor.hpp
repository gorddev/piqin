#pragma once

#include "engine/types/Gear.hpp"
#include "engine/scene/animation/AnimInfo.hpp"

namespace geng {
    /**
     * @brief A @code gengine::Actor@endcode is a simple game entity that carries with it a @code gengine::Transform@endcode
     * and a @code gengine::FrameState@endcode.
     * It is one of the primary three objects the engine performs operations on (that being ParticleGroups, Actors, and Panels).
     * @details The Actor is a subclass of the @code geng::Gear@endcode, which means that it inherits many of their nice features.
     * Refer to the Gear documentation for more details in engine/types. The Actor, although it can be used alone, is primarily intended
     * to be taken as a parent class you derive subclasses from. This way, you can use many of the very nice virtual functions of an Actor:
     * - @code virtual void update()@endcode › Called every frame, so if you want frame-dependent behavior like velocity, put it in this function.
     * - @code virtual void hover_release()@endcode › Called when the object is no longer hovered over
     * - @code virtual void on_hover()@endcode › Called when the object is hovered over
     * - @code virtual void on_click()@endcode › Called when the object is clicked on
     * - @code virtual void click_release()@endcode › Called when the click is release on this object
     * - @code virtual void move(geng::Vertex dist)@endcode › Called when the cursor drags the object any distance @code dist@endcode.
     * - @code AnimInfo anim@endcode › Contains all relevant animation info. It is recommended for specific Actor subclasses, you
     * predefine the frame_table_id and animation_index, such that each Actor subclass is tied to a specific animation set. Refer to the
     * AnimInfo documentation for more info
     */
    struct Actor : public geng::Gear {
    public:
        /// An actor's animation information.
        AnimInfo anim;

    public:
        /// Default Constructor
        Actor() { set_shadow(); };
        /// Default copy constructor
        Actor(Actor& o) = default;
        /// Defines an Actor with a starting position
        explicit Actor(Vertex pos) : Gear(pos) { set_shadow(); }
        /// Defines an actor with a starting position and animation info.
        Actor(Vertex pos, AnimInfo anim) : Gear(pos), anim(anim) { set_shadow(); }

        /// Virtual update function
        virtual void update(LayerTime& time) {}
        /// Virtual Destructor
        ~Actor() override = default;

        /// To String
        [[nodiscard]] std::string to_string() const override {
            return	"Actor:" + Gear::to_string() + " " + t.to_string() + "\n" +  anim.to_string();
        }

        // Manual overrides for Gears
        /// Converts an actor to buffer
        void to_vertex(RenderBuffer &buffer) override {
            anim.calc_vertices(buffer, this);
            if (has_shadow())
                buffer.push_shadow(6);
        }
        /// Returns the z-index of
        [[nodiscard]] float z_index() const override {
            return t.pos.z;
        }

        /// This function is called when hovering over the actor with a cursor
        void on_hover() override { std::cerr << "Actor::on_hover()" << std::endl; }
        /// This function is caleld when the is_hoverable is pulled away from the actor
        void on_hover_release() override {}
        /// This function is called when the object is is_clicked on
        void on_click() override {}
        /// This function is called when the click for this object is released
        void on_click_release() override {}
        /// This function is called by the input manager when moving an object.
        void move(Vertex dist) override {}
    };
}
