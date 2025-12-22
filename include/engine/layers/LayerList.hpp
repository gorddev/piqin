#pragma once
#include <utility>

#include "LayerMap.hpp"
#include "Layer.hpp"

namespace geng {

    /** LayerList keeps track of all of the layers and allows binding of string names to layers.**/
    class LayerList {
    private:
        /// Links indexes to Layers
        std::vector<Layer*> layers;
        /// Topmost id of the layer
        int active_id = -1;
        /// Keeps track of the EngineContext
        EngineContext& engine_context;
    public:
        explicit LayerList(EngineContext& world) : engine_context(world) {}

        /// Updates each of the layers
        void update(double dt) {
            for (auto& l : layers) {
                if (l->scene.is_running())
                    l->update(dt);
            }
        }

        /// Gets a layer based on layer name
        Layer* get_layer(std::string name) const {
            for (auto& l : layers) {
                if (l->scene.get_name() == name)
                    return l;
            }
            engine_context.log(1, "Cannot get layer, as layer " + name + " is not composed in the engine.", "LayerList::getLayer()");
            return nullptr;
        }

        /// Returns the active layer
        Layer* get_active_layer() {
            if (active_id == -1) {
                engine_context.log(1, "No active layer currently set.", "LayerList::get_active_layer()");
                return nullptr;
            }
            return layers[active_id];
        }

        /// Sets the active layer based on layer pointer
        void set_active_layer(Layer* l) {
            set_active_layer(l->scene.get_name());
        }

        /// Sets the active layer based on layer name
        void set_active_layer(const std::string& name) {
            if (get_active_layer() != nullptr)
                get_active_layer()->_set_active(false);
            for (int i = 0; i < (int)layers.size(); i++) {
                if (layers[i]->scene.get_name() == name) {
                    active_id = i;
                    layers[i]->_set_active(true);
                    return;
                }
            }
            engine_context.log(1, "Layer " + name + " not composed. Cannot set as active.", "LayerList::set_active_layer()");
            if (get_active_layer() != nullptr)
                get_active_layer()->_set_active(false);
        }

        /// Adds a layer to the LayerList
        void add_layer(Layer* l) {
            l->_add_engine_context(&engine_context);
            l->scene.id = layers.size();
            layers.push_back(l);
            if (active_id == -1) {
                l->_set_active(true);
                set_active_layer(l);
            }
        }

        /// Increments the current active layer by 1 (loops back to)
        void increment_active_layer() {
            if (active_id != -1) {
                layers[active_id]->_set_active(false);
                active_id++;
                if (active_id == layers.size())
                    active_id = 0;
                layers[active_id]->_set_active(true);
            }
        }

        /// Removes a layer from the LayerList
        void remove_layer(const std::string& name) {
            bool was_active = false;
            bool found = false;
            // Remove it from the vector
            for (auto it = layers.begin(); it != layers.end(); ++it) {
                if ((*it)->scene.get_name() == name) {
                    if ((*it)->is_active())
                        was_active = true;
                    found = true;
                    delete (*it);
                    layers.erase(it);
                }
            }
            if (found == false) {
                engine_context.log(1, "Error: Layer "+ name + " not found.", "LayerList::remove_layer()");
                return;
            }
            // Now we need to reassign layer map ids because indexes have changed.
            for (int i = 0; i < (int)layers.size(); i++)
                layers[i]->scene.id = i;
            if (was_active)
                if (layers.size() > 0)
                    set_active_layer(layers[0]);
        }

        /// Removes a layer from the LayerList via pointer
        void remove_layer(const Layer* l) {
            remove_layer(l->scene.get_name());
        }

        /// Moves layer to front of order
        void move_to_front(Layer* l) {
            auto it = std::find(layers.begin(), layers.end(), l);
            if (it == layers.end() || it == layers.end() - 1)
                return;

            int old_index = it - layers.begin();

            layers.erase(it);
            layers.push_back(l);

            // Shift everything down
            for (int i = old_index; i < (int)layers.size(); ++i)
                layers[i]->scene.id = i;
        }

        /// Move a layer to the back (bottom of rendering order)
        void move_to_back(Layer* l) {
            auto it = std::find(layers.begin(), layers.end(), l);
            if (it == layers.end() || it == layers.begin())
                return;

            int old_index = it - layers.begin();

            layers.erase(it);
            layers.insert(layers.begin(), l);

            // Shift everything up
            for (int i = 0; i <= old_index; ++i)
                layers[i]->scene.id = i;
        }

        /// Move a layer forward by one in rendering order
        void move_forward(Layer* l) {
            auto it = std::find(layers.begin(), layers.end(), l);
            if (it == layers.end() || it == layers.end() - 1)
                return;

            Layer* a = *it;
            Layer* b = *(it + 1);

            std::iter_swap(it, it + 1);
            std::swap(a->scene.id, b->scene.id);
        }

        /// Move a layer backward by one in rendering order
        void move_backward(Layer* l) {
            auto it = std::find(layers.begin(), layers.end(), l);
            if (it == layers.end() || it == layers.begin())
                return;

            Layer* a = *it;
            Layer* b = *(it - 1);

            std::iter_swap(it, it - 1);
            std::swap(a->scene.id, b->scene.id);
        }

        /// Gets the list of layers
        std::vector<Layer*>& get_layer_list() {
            return layers;
        }

    };
}
