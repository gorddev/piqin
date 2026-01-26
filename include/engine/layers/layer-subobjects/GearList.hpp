#pragma once

#include "engine/core/gears/Gear.hpp"

namespace gan {

    /// function that ties to the ModuleManager's gear removal function
    using GearRemovalFunction  = void (*)(void*, Gear*, uint_fast16_t);

    /// Contains all the internal gears
    class GearList {
    private:
        /// Internal vector that keeps track of each gear
        std::vector<Gear*> gears;
        /// Maps Gear* to pair<module_id, vector_index>>
        std::unordered_map<Gear*, std::pair<uint_fast16_t, int>> gear_map;
        /// Points to the module_manager
        void* module_manager;
        /// Points to the notify_gear_removal function in ModuleManager
        GearRemovalFunction gear_removal_function;
        /// Contains the id of the layer
        const int& layer_id;

        /// Layer is a friend class
        friend class Layer;
        friend class LayerCore;

        /// Private constructor
        explicit GearList(GearRemovalFunction bind, int& layer_id, void* module_manager);
        /// Private gear operator[]
        Gear*& operator[](uint_fast32_t index);
    public:
        /// Public destructor
        ~GearList();
        // <><><> Adding & Removing <><><>
        /// Adds a gear to the gear list
        bool add_gear(Gear* g, const uint_fast16_t& module_id);
        /// Adds several gears to the gear list
        template<typename T> requires(std::derived_from<T, Gear>)
        void add_gears(std::vector<T*> gear_list, const uint_fast16_t& module_id);
        /// Removes a gear from the gear list
        void remove_gear(Gear* g);

        // <><><> Sorting <><><>
        /// Sorts all gears based on z_index
        void sort_gears();

        // <><><> Rendering <><><>
        /// Renders all gears with the provided render buffer
        void render_gears(RenderBuffer& buffer);

        void render_hitboxes(RenderBuffer &buffer, uint16_t cam_width) const;
    };

    template<typename T> requires (std::derived_from<T, Gear>)
    void GearList::add_gears(std::vector<T*> gear_list, const uint_fast16_t& module_id) {
        // reserve enough space for the new gears
        gears.reserve(gears.size() +gear_list.size());
        // adds gears to the gear list
        for (auto& g : gear_list)
           add_gear(g);
    }
}
