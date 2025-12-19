#pragma once

#include "engine/types/Gear.hpp"
#include "engine/scene/animation/AnimInfo.hpp"
#include "engine/scene/animation/FrameTable.hpp"

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
        /// An actor's frameTable
        FrameTable& frameTable;

    public:
        /// Defines an actor with a given frametable, sets default animation to 0.
        explicit Actor(FrameTable& frames);
        /// Defines an actor with a given frametable and a default animation
        explicit Actor(FrameTable& frames, uint16_t default_animation);
        /// Defines an Actor with a starting position
        explicit Actor(FrameTable& frames, uint16_t default_animation, Transform t);
        /// Default copy constructor. Here for readability
        Actor(Actor& o) = default;

        /// Virtual update function
        virtual void update(LayerTime& time) {}

        /// Non-virtual update_frame function.
        void update_frame(LayerTime& time);

        /// Virtual Destructor
        ~Actor() override = default;

        /// To String
        [[nodiscard]] std::string to_string() const override;

        // <><>< Manual overrides for Gears <><><>
        /// Converts an actor to vertex for the buffer. overridable, but not recommended to do so.
        void to_vertex(RenderBuffer &buffer) override;
        /// Returns the z-index of the actor (the z-coordinate in their transform.pos object). overridable, but not recommended to do so unless one of your element has a specialized z-index.
        [[nodiscard]] float z_index() const override;

        // Input handling
        /// This function is called when hovering over the actor with a cursor
        void on_hover() override {}
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