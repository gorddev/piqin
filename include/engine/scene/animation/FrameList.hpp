#pragma once
#include <unordered_map>
#include <iostream>


#include "FrameTable.hpp"
#include "Mesh.hpp"
#include "../layers/LayerContext.hpp"
#include "engine/scene/actors/Actor.hpp"


namespace geng {
    /** @brief This is essentially a wrapper class for a vector of FrameTables that only allows adding and retrieval **/
    /** @details Member functions:
     * - @code add_tables(const std::vector<FrameTable>& frameTables)@endcode › adds a vector of frametables to the manager.
     * - @code get_table(int id)@endcode › Returns the associated table with the id.
     */
    class FrameList {
    private:
        /// Links frame_table_ids in AnimInfo actors to FrameTables.
        std::vector<FrameTable> tables;
        /// Allows for debugging
        LayerContext& scene;

    public:
        /// Constructs the animation manager.
        explicit FrameList(LayerContext& scene);
        /// Loads the FrameSheets found in the FrameRegister
        void add_tables(const std::vector<FrameTable>& frameTables);
        /// Gets the FrameTable associated with a specific actor.
        FrameTable& get_table(int id);

        /// Gets the beginning interator of the vector
        auto begin();
        /// Gets the ending iterator of the vector
        auto end();
    };
}
