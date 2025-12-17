#pragma once

#include <iostream>
#include <vector>
#include "EngineEnums.hpp"
#include "Transform.hpp"
#include "engine/rendering/RenderBuffer.hpp"

namespace geng {
    /** @brief A gear is the fundamental abstract base class the engine handles. Actors, ParticleGroups, and Panels are all subclasses of the Gear class.
     * @details Member variables and classes are listed below:
     * - @code int id@endcode The id of the Gear
     * - @code GFlag flag@endcode Contains rendering information, like hidden, shadow, ect.
     * - @code virtual float z_index() = 0@endcode Returns the z-index of the object
     * - @code virtual void to_vertex() = 0@endcode Gets the rendering vertices of the object
     * - @code virtual ~Gear() = default@endcode Virtual destructor.
     */
    struct Gear {
    private:
        /// Rendering flag for the gear
        GFlag flag = GFlag::locked | GFlag::shadow;
    public:
        /// The transform of any gear
        Transform t;
        /// Id of the Gear. Default is -1, but it should be reassigned.
        int id = -1;
        /// Texture id of the gear. The -1 specification tells the renderer to render it as completely white.
        int texture_id = -1;

        /// Default constructor
        Gear() = default;
        /// Establish a pos
        explicit Gear(const Vertex &pos) : t(pos) {}
        /// Constructor with a transform
        explicit Gear(Transform t) : t(std::move(t)) {}

        /// Z_index of the gear
        virtual float z_index() const = 0;
        /// Gets the rendering buffer of the gear
        virtual void to_vertex(RenderBuffer& buffer) = 0;
        /// Virtual destructor of the gear.
        virtual ~Gear() = default;
        /// Virtual to_string for the gear
        virtual std::string to_string() const {
            return "Id: " + std::to_string(id) + "\nFlag: " + geng::to_string(flag);
        }

        /* ............. */
        /* Bitmask helpers for easy flagging */
        /* ............. */
        // Locking
        /// Prevents complex transformation calculations from taking place
        void lock() { flag |= GFlag::locked; }
        /// Unflips the lock flag, such that complex render calculations like rotation will be performed on the object
        void unlock() { flag &= ~GFlag::locked;}
        /// Returns if the flag is locked or not
        bool locked() const { return flag << GFlag::locked; }

        // Hiding
        /// Hides the object
        void hide() { flag |= GFlag::hidden; }
        /// Unhides the object
        void unhide() { flag &= ~GFlag::hidden; }
        /// Returns true if the object is hidden
        bool hidden() const { return flag << GFlag::hidden; }

        // Flipping
        /// Flips the object horizontally
        void flipX() { flag |= GFlag::flipX; }
        /// Flip the object vertically
        void flipY() { flag |= GFlag::flipY; }
        /// Unflips the object horizontally
        void unflipX() { flag &= ~GFlag::flipX; }
        /// Unflips the object vertically
        void unflipY() { flag &= ~GFlag::flipY; }
        /// Returns true if flipped horizontally
        bool flippedX() const { return flag << GFlag::flipX; }
        /// Returns true if flipped vertically
        bool flippedY() const { return flag << GFlag::flipY; }

        // Shadows
        /// Displays a shadow for this object
        void set_shadow() { flag |= GFlag::shadow; }
        /// Prevents the display of a shadow for this object
        void unset_shadow() { flag &= ~GFlag::shadow; }
        /// Returns true if the object should have a shadow rendered
        bool has_shadow() const { return flag << GFlag::shadow; }

        // Removal
        /// Flags the object to be removed in the next frame
        void flag_removal() { flag |= GFlag::remove; }
        void unflag_removal() { flag &= ~GFlag::remove; }
        /// Returns true if the object is flagged for removal
        bool remove() const { return flag << GFlag::remove; }

        // Tagging
        /// The object's tostring will be printed out every frame
        void tag() { flag |= GFlag::tagged; }
        /// The object's tostring will not be printed out every frame
        void untag() { flag &= ~GFlag::tagged; }
        /// Returns true if is_tagged
        bool is_tagged() const { return flag << GFlag::tagged; }

        // Gives the type of the object
        /// Returns true if the object is an actor
        bool is_actor() const { return flag << GFlag::actor; }
        /// Returns true if the object is a particle
        bool is_particle() const { return flag << GFlag::particle; }
        /// Returns true if the object is a panel
        bool is_panel() const { return flag << GFlag::panel; }

        /* .... Clicking and dragging .... */
        // Clicking
        /// Returns true if the object is currently is_clicked
        bool is_clicked() { return flag << GFlag::clicked; }
        /// Returns true if the object is currently being dragged.
        bool is_dragged() const { return flag << GFlag::dragged; }
        /// Returns true if the object is currently hovered over
        bool is_hovered() const { return flag << GFlag::hovered; }

        // Dragging
        /// Specifies this gear can be dragged
        void set_draggable() { flag |= GFlag::draggable; }
        /// Specifies this Gear can no longer be dragged
        void unset_draggable() { flag &= ~GFlag::draggable; }
        bool is_draggable() const { return flag << GFlag::draggable; }

        // For engine use exclusively. DO NOT USE
        /// Adds special flags to the Gear
        void _engine_flagger(GFlag engine_flag) { flag |= engine_flag; }
        /// Removes special flag from the gear
        void _engine_deflagger(GFlag engine_deflag) { flag &= ~engine_deflag; }

        /// This function is called when the gear is designated as a keyboard_accepter
        virtual void on_key_press(SDL_Scancode key) { std::cerr << "Gear::on_keypress: " << key << std::endl; }
        /// This function is called when the gear is designated as a keyboard acceptor and a key is released
        virtual void on_key_release(SDL_Scancode key) { std::cerr << "Gear::on_keypress: " << key << std::endl; }
        /// This function is called when hovering over the actor with a cursor, and they are designated as a mouse acceptor
        virtual void on_hover() { std::cerr << "Gear::on_hover()" << std::endl; }
        /// This function is caleld when the is_hoverable is pulled away from the actor
        virtual void on_hover_release() {}
        /// This function is called when the object is is_clicked on
        virtual void on_click() {}
        /// This function is called when the click for this object is released
        virtual void on_click_release() {}
        /// This function is called by the input manager when moving an object.
        virtual void move(Vertex dist) {}
    };
}
