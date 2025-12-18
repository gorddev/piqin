#pragma once
#include <utility>

#include "LayerMap.hpp"
#include "engine/scene/Layer.hpp"

namespace geng {

    /** LayerStack keeps track of all of the layers and allows binding of string names to layers.**/
    class LayerStack {
    private:
        /// Links indexes to Layers
        std::vector<Layer*> layers;
        /// Topmost id of the layer
        int top_id = 0;
        /// Links string names to indexs in the vector, and allows Layers to edit the active layer
        LayerMap layerMap;
        /// Keeps track of the EngineContext
        EngineContext& world;
    public:
        explicit LayerStack(EngineContext& world) : world(world) {}

        /// Updates each of the layers
        void update(double dt) {
            for (auto& l : layers) {
                if (l->scene.is_running())
                    l->update(dt);
            }
        }

        /// Gets a layer based on layer name
        Layer* get_layer(std::string name) {
            return layers[layerMap.get_layer_id(std::move(name))];
        }

        /// Returns the active layer
        Layer* get_active_layer() {
            if (layerMap.get_active_layer_index() == -1)
                return nullptr;
            return layers[layerMap.get_active_layer_index()];
        }

        /// Sets the active layer based on layer pointer
        void set_active_layer(Layer* l) {
            layerMap.switch_layer(l->scene.get_name());
        }
        /// Sets the active layer based on layer name
        void set_active_layer(const std::string& name) {
            layerMap.switch_layer(name);
        }

        /// Switches the active layer to the name
        void switch_layer(const std::string& name) {
            layers[layerMap.get_active_layer_index()]->scene._engine_deflagger(LayerFlag::active);
            layerMap.switch_layer(name);
            layers[layerMap.get_active_layer_index()]->scene._engine_flag(LayerFlag::active);
        }

        /// Adds a layer to the LayerStack
        void add_layer(Layer* l) {
            l->_add_engine_context(&world);
            layers.push_back(l);
            l->scene.id = top_id++;
            layerMap._add_layer(l->scene.get_name(), l->scene.id);
        }

        /// Removes a layer from the LayerStack
        void remove_layer(const std::string& name) {
            if (!layerMap.layer_exists(name)) {
                std::cerr << "Error: Layer " << name << " does not exist." << std::endl;
                return;
            }
            // Remove it from the layer map
            layerMap.get_layer_id(name);
            // Remove it from the vector
            for (auto it = layers.begin(); it != layers.end(); ++it) {
                if ((*it)->scene.get_name() == name) {
                    delete (*it);
                    layers.erase(it);
                    break;
                }
            }
            // Now we need to reassign layer map ids because indexes have changed.
            Layer* active_layer = nullptr;
            // We get the current active layer so we can reassign it after clearing our
            if (layerMap.get_active_layer_index() != -1)
                active_layer = layers[layerMap.get_active_layer_index()];
            layerMap.clear();
            // Reassign layer ids
            for (int i = 0; i < (int)layers.size(); i++) {
                layerMap._add_layer(layers[i]->scene.get_name(), i);
                if (layers[i]==active_layer) {
                    layerMap.switch_layer(layers[i]->scene.get_name());
                }
            }
        }

        /// Removes a layer from the LayerStack via pointer
        void remove_layer(const Layer* l) {
            if (!layerMap.layer_exists(l->scene.get_name())) {
                world.log(1, "Layer " + l->scene.get_name() + " does not exist in Engine.\n", "LayerStack::remove_layer");
                return;
            }
            remove_layer(l->scene.get_name());
        }

        /// Move a layer to the front (top of rendering order)
        void move_to_front(Layer* l) {
            auto it = std::find(layers.begin(), layers.end(), l);
            if (it != layers.end()) {
                layers.erase(it);
                layers.push_back(l);
            }
        }

        /// Move a layer to the back (bottom of rendering order)
        void move_to_back(Layer* l) {
            auto it = std::find(layers.begin(), layers.end(), l);
            if (it != layers.end()) {
                layers.erase(it);
                layers.insert(layers.begin(), l);
            }
        }

        /// Move a layer forward by one in rendering order
        void move_forward(Layer* l) {
            auto it = std::find(layers.begin(), layers.end(), l);
            if (it != layers.end() && it != layers.end() - 1) {
                std::iter_swap(it, it + 1);
            }
        }

        /// Move a layer backward by one in rendering order
        void move_backward(Layer* l) {
            auto it = std::find(layers.begin(), layers.end(), l);
            if (it != layers.end() && it != layers.begin()) {
                std::iter_swap(it, it - 1);
            }
        }

        /// Gets the list of layers
        std::vector<Layer*>& get_layer_list() {
            return layers;
        }

    };
}
