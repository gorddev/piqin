#pragma once

#include "../../core/gears/Gear.hpp"
#include "engine/layers/LayerState.hpp"

namespace gan {
    /**
  * @brief Morphs attach themselves to Gear objects, so any Gear can have an morph applied to it, although it may not have any visible change.
  * Primary candidates for morphs are @code gan::Banners@endcode and @code gan::Sprites@endcode.
  * @details To create a new morph, make a subclass of the morph object and add it via the Engine.
  * Morphs have six member variables:
  * 1. Transform2D* t -> pointer to the object's Transform2D [protected]
  * 2. float initDuration -> initial duration of the morph (-1 implies permanent) [protected]
  * 3. float duration -> current duration of the morph [protected]
  * 4. bool permanent -> if duration is set to -1, permanent is set to true in the morph class. [protected]
  * 5. int id -> for engine identification and speed purposes [private]
  * 6. int target_id -> lets us remove morph by object ID, since Objects don't have an morph pointer [private]
  *
  * @note Notes for making morphs:
  * - Morph is a pure virtual class
  * - Morph's constructors will initialize the initDuration and permanent fields for you
  * - Add morphs to the MorphManager with @code engine.add_morph(new gan::Morph(...)) @endcode
  * - Remove permanent morphs with @code engine.remove_morph(Morph *e) @endcode or @code engine.remove_morph(Gear* g) @endcode
  * - Unless you explicitly program it, Morphs will not return an object to its original transform upon completion
  *
  * @warning To make an morph you must override the update() function. Update() returns true if the morph is done, false otherwise.
  */
    class Morph {
    protected:
        /// Transform2D object the morph is applied to
        Gear* gear = nullptr;
        /// Initial duration of the morph.
        float initDuration;
        /// Current duration of the morph.
        float duration;
        /// Amplitude of the morph
        float amplitude = 1;
        /// If the morph is permanent
        bool permanent;
    private:
        /// For engine identification purposes.
        int id = -1;
    public:
        /// Constructor without transform (permanent)
        Morph()
            : initDuration(0), duration(0), permanent(true) {}
        /// Constructor without transform (duration-limited)
        explicit Morph(float duration)
            : initDuration(duration), duration(duration), permanent(duration == -1) {}
        /// Constructor without transform (duration-limited and amplitude)
        explicit Morph(float duration, float amplitude)
            : initDuration(duration), duration(duration), amplitude(amplitude), permanent(duration == -1) {}
        /// Constructor for Permanent Morph
        explicit Morph(Gear* t)
            : gear(t), initDuration(0), duration(0), permanent(true) {}
        /// Constructor for duration-bound morph
        Morph(Gear* t, float duration)
            : gear(t), initDuration(duration), duration(initDuration), permanent(duration == -1) {}
        /// Constructor for a duration and an amplitude
        Morph(Gear* t, float duration, float amplitude)
            : gear(t), initDuration(duration), duration(duration),
                    amplitude(amplitude), permanent(duration == -1) {}

        /// Virtual destructor cause i'm considerate like that
        virtual ~Morph() = default;
        /// Pure virtual update function. Return true if morph is done. False otherwise.
        virtual bool update(LayerState& time) = 0;
        /// Immmediately ends an morph
        virtual void end() { duration = 0; permanent = false;};

        // ............... //
        // For use by the engine //
        // ............... //
        /// Sets the transform the morph targets.
        void set_gear(Gear* g) { gear = g; }
        /// Sets the ID of the morph
        void set_id(int i) { id = i; }
        /// Gets the ID of the morph
        int get_id () const { return id; }
        /// Gets the id of the associated Gear
        int get_gear_id() const { return gear->id; }
        /// Gets the associated gear pointer
        Gear* get_gear_ptr() const { return gear; }
        /// Returns if the morph is permanent or not
        bool is_permanent () const { return permanent; }
    };
}
