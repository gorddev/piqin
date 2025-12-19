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

        bool switch_layer(const std::string& name) {
            if (!layer_exists(name)) {
                return false;
            }
            activeLayer = string_to_index[name];
            return true;
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

        int get_layer_id(std::string name) {
            if (layer_exists(name)) {
                return string_to_index[name];
            }
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

        void clear() {

        }

    };
}
