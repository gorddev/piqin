#pragma once

#include "Effect.hpp"
#include "engine/utilities/types/SparseVector.hpp"
#include "engine/objects/Object.hpp"

namespace gengine {
    /// EffectManager class. For explicit and exclusive use by the engine.
    /// Handles updating each effect pointer, and removing effects when completed.
    /// EffectManager uses a SparseVector class for removal/insertion efficiency.
    class EffectManager final {
    private:
        /// Vector containing each effect pointer
        utils::SparseVector<Effect> effects;
    public:
        /// Default constructor
        EffectManager() = default;
        /// Default destructor
        ~EffectManager() = default;

        // Add effects
        /// Adds an effect to a given object
        void add_effect(Object& o, Effect* e);
        /// Adds an effect to a given transform
        void add_effect(Transform& t, Effect* e);

        // Remove effects
        /// Removes all effects on an object (by searching through target_id)
        void remove_effect(Object& o);
        /// Removes a singular effect by Effect*
        void remove_effect(Effect* e);

        // Updater
        /// Updates all effects in the effect manager. Removes finished effects.
        void update();

    };
}