#pragma once

#include <iostream>
#include <unordered_map>

namespace geng {

    /** Links strings to layer ids **/
    class LayerMap {
    private:
        /// Links string to layer ids
        std::unordered_map<std::string, int> string_to_index;
        /// Keeps track of the active layer.
        int activeLayer = -1;
    public:
        /// Default constructor
        LayerMap() = default;
        ~LayerMap() = default;

        [[nodiscard]] int get_active_layer_index() const {
            return activeLayer;
        }

        void switch_layer(const std::string& name) {
            if (!layer_exists(name)) {
                std::cerr << "Err: Layer " << name << " does not exist." << std::endl;
                return;
            }
            activeLayer = string_to_index[name];
        }

        /// Adds one layer to the layer map
        void _add_layer(std::string name, int index) {
            string_to_index[name] = index;
            if (activeLayer == -1) {
                activeLayer = index;
            }
        }

        /// Returns true if the name does exist
        bool layer_exists(std::string name) {
            return string_to_index.find(name) != string_to_index.end();
        }

        int pop_layer(std::string name) {
            if (layer_exists(name)) {
                int id = string_to_index[name];
                string_to_index.erase(name);

                if (id == activeLayer) {
                    activeLayer = -1;
                    for (auto& [name, index] : string_to_index) {
                        activeLayer = index;
                        break;
                    }
                }

                return id;
            }
            std::cerr << "Err: Layer " << name << " does not exist." << std::endl;
            return -1;
        }

        void remove_layer(int index) {
            if (activeLayer == index)
                activeLayer = -1;
            for (auto& [str, ind] : string_to_index) {
                if (ind == index) {
                    string_to_index.erase(str);
                    break;
                }
            }
        }

    };
}
