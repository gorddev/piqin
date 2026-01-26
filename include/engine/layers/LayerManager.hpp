#pragma once

#include "Layer.hpp"

namespace gan {

    /** LayerManager keeps track of all of the layers and allows binding of string names to layers.**/
    class LayerManager {
    private:
        /// Links indexes to Layers
        std::vector<Layer*> layers;
        /// Topmost id of the layer
        int active_id = -1;
        /// Keeps track of the EngineContext
        EngineContext& engine_context;

    public:
        explicit LayerManager(EngineContext& world);

        // <><><> Layer updating <><><>
        /// Updates each of the layers
        void update(double dt);

        // <><><> Adding layer <><><>
        /// Adds a layer to the LayerManager
        void add_layer(Layer* l);

        // <><><> Layer Retrieval <><><>
        /// Gets a layer based on layer name
        Layer* get_layer(gan::fstring<10> name);
        /// Returns the active layer
        Layer* get_active_layer();

        // <><><> Setting Active Layer
        /// Sets the active layer based on layer pointer
        void set_active_layer(Layer* l);
        /// Sets the active layer based on layer name
        void set_active_layer(gan::fstring<10> name);
        /// Increments the current active layer by 1 (loops back to)
        void increment_active_layer();

        // <><><> Removing Layers <><><>
        /// Removes a layer from the LayerManager
        void remove_layer(gan::fstring<10> layer_name);
        /// Removes a layer from the LayerManager via pointer
        void remove_layer(const Layer* l);

        // <><><> Reordering Layers <><><>
        /// Moves layer to front of order
        void move_to_front(Layer* l);
        /// Move a layer to the back (bottom of rendering order)
        void move_to_back(Layer* l);
        /// Move a layer forward by one in rendering order
        void move_forward(Layer* l);
        /// Move a layer backward by one in rendering order
        void move_backward(Layer* l);

        // <><><> Layer Verification
        /// Gets the list of layers
        std::vector<Layer*>& get_layer_list();
        /// Returns true if the manager has the layer currently
        bool has_layer(Layer* l);
        /// Returns true if the manager currently has the layer.
        bool has_layer(fstring<10> name);
    };

}