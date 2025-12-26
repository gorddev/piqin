#pragma once

#include "engine/scene/cells/Cell.hpp"

namespace geng {

    /** Cell bucket is a testing class to test imaging of textures. They should only be used for debug. */
    class CellBucket {
        gch::vector<Cell> cells;
    public:
        CellBucket() = default;

        void add_cell(int tex_id, SDL_FPoint tex_start = {0.f, 0.f}, SDL_FPoint tex_end = {0.f, 0.f}) { cells.emplace_back(tex_id, tex_start, tex_end); }
        gch::vector<Cell>& get_cell_list() { return cells; }
        auto begin() { return cells.begin(); }
        auto end() { return cells.end(); }
        auto data() { return cells.data();}
    };
}
