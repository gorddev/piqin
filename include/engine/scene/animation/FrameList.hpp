#pragma once
#include <unordered_map>


#include "FrameTable.hpp"
#include "Mesh.hpp"
#include "engine/layers/LayerContext.hpp"
#include "engine/scene/sprites/Sprite.hpp"


namespace geng {
    /** @brief This is essentially a wrapper class for a vector of FrameTables that only allows adding and retrieval **/
    /** @details Member functions:
     * - @code add_tables(const lni::vector<FrameTable>& frameTables)@endcode › adds a vector of frametables to the manager.
     * - @code get_table(int id)@endcode › Returns the associated table with the id.
     */
    class FrameList {
    private:
        /// Links frame_table_ids in AnimInfo sprites to FrameTables.
        gch::vector<FrameTable> tables;

    public:
        /// Constructs the animation manager.
        explicit FrameList() = default;

        int add_table(const FrameTable &table);

        /// Loads the FrameSheets found in the FrameRegister
        void add_tables(gch::vector<FrameTable>& frameTables);
        /// Gets the FrameTable associated with a specific sprites.
        FrameTable& get_table(int id);

        /// Gets the beginning interator of the vector
        auto begin();
        /// Gets the ending iterator of the vector
        auto end();
    };
}
