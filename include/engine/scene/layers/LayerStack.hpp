#pragma once
#include <unordered_map>
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

        /// Returns the active layer
        Layer* get_active_layer() {
            if (layerMap.get_active_layer_index() == -1)
                return nullptr;
            return layers[layerMap.get_active_layer_index()];
        }

        void set_active_layer(Layer* l) {
            layerMap.switch_layer(l->scene.get_name());
        }
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
            layerMap.pop_layer(name);
            // Remove it from the vector
            for (auto it = layers.begin(); it != layers.end(); ++it) {
                if ((*it)->scene.get_name() == name) {
                    delete (*it);
                    layers.erase(it);
                    break;
                }
            }
        }

        /// Removes a layer from the LayerStack via pointer
        void remove_layer(const Layer* l) {
            int id = -1;
            // Search through layers
            for (auto it = layers.begin(); it != layers.end(); ++it) {
                // If we find it
                if (l == (*it)) {
                    // Pop it from the layer map
                    layerMap.pop_layer((*it)->scene.get_name());
                    // Delete it
                    delete (*it);
                    // Erase it from the vector of layers
                    layers.erase(it);
                    // Return
                    return;
                }
            }
            std::cerr << "Layer " << l << " not found in layer stack.\n";
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
