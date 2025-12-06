#pragma once

#include "engine/objects/Transform.hpp"

namespace gengine {
    /**
  * @brief Effects attach themselves to Transform objects, so any type with a Transform can have an effect applied to it.
  * @details To create a new effect, make a subclass of the effect object and add it via the Engine.
  * Effects have six member variables:
  * 1. Transform* t -> pointer to the object's Transform [protected]
  * 2. float initDuration -> initial duration of the effect (-1 implies permanent) [protected]
  * 3. float duration -> current duration of the effect [protected]
  * 4. bool permanent -> if duration is set to -1, permanent is set to true in the effect class. [protected]
  * 5. int id -> for engine identification and speed purposes [private]
  * 6. int target_id -> lets us remove effect by object ID, since Objects don't have an effect pointer [private]
  *
  * @note Notes for making effects:
  * - Effect is a pure virtual class
  * - Effect's constructors will initialize the initDuration and permanent fields for you
  * - Add effects to the EffectManager with @code engine.add_effect(Object* o, Effect* e) @endcode
  * - Remove permanent effects with @code engine.remove_effect(Effect *e) @endcode or @code engine.remove_effect(Object* o) @endcode
  * - Each Effect at base takes up 36 bytes (8 + 4 + 4 + 1 [3 padding] + 4 + 4 + 8 for virtual funcs)
  * - Unless you explicitly program it, Effects will not return an object to its original transform upon completion
  *
  * @warning To make an effect you must override the update() function. Update() returns true if the effect is done, false otherwise.
  */
    class Effect {
    protected:
        /// Transform object the effect is applied to
        Transform* t = nullptr;
        /// Initial duration of the effect.
        float initDuration;
        /// Current duration of the effect.
        float duration;
        /// If the effect is permantn
        bool permanent;
    private:
        /// For engine identification purposes.
        int id = -1;
        /// The id of the target object (used for removal of effects by object id)
        int target_id = -1;
    public:
        // Constructor without transform (permanent)
        Effect()
            : initDuration(0), duration(0), permanent(true) {}
        // Constructor without transform (duration-limited)
        Effect(float duration)
            : initDuration(duration), duration(duration), permanent(duration == -1) {}
        // Constructor for Permanent Effect
        Effect(Transform& t)
            : t(&t), initDuration(0), duration(0), permanent(true) {}
        // Constructor for duration-bound effect
        Effect(Transform& t, float duration)
            : t(&t), initDuration(duration), duration(initDuration), permanent(duration == -1) {}
        // Virtual destructor cause i'm considerate like that
        virtual ~Effect() = default;

        // Pure virtual update function. Return true if effect is done. False otherwise.
        virtual bool update() = 0;

        // For use by the engine
        // Setting the transform
        void set_transform(Transform& transform) { t = &transform; }
        // Effect IDs
        void set_id(int i) { id = i; }
        [[nodiscard]] int get_id () const { return id; }
        // Target IDs
        void set_target_id(int i) { target_id = i; }
        [[nodiscard]] int get_target_id () const { return target_id; }

    };
}