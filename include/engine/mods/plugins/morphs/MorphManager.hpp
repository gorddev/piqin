#pragma once

#include "Morph.hpp"
#include "../../layers/layer-subobjects/LayerCore.hpp"
#include "engine/utilities/SparseVector.hpp"

namespace gan {
    /// MorphManager class. For explicit and exclusive use by the engine.
    /// Handles updating each morph pointer, and removing morphs when completed.
    /// MorphManager uses a SparseVector class for removal/insertion efficiency.
    class MorphManager final {
    private:
        /// Vector containing each morph pointer
        gutils::SparseVector<Morph> morphs;
        /// Contains the LayerContext
        LayerCore& scene;
    public:
        /// Default constructor
        MorphManager(LayerCore& layer_context);
        /// Default destructor
        ~MorphManager() = default;

        // Add morphs
        /// Adds an morph to the manager
        void apply_morph(Morph *e);

        // Remove morphs
        /// Removes all morphs on a gear (by searching through target_id)
        void strip_morph(const Gear *g);
        /// Removes a singular morph by Morph*
        void strip_morph(Morph* e);

        // Updater
        /// Updates all morphs in the morph manager. Removes finished morphs.
        void update();

        /// Check if a gear has a morph
        bool has_morph(const Gear* g);

    };
}