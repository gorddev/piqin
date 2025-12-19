#include <iostream>
#include "engine/scene/animation/FrameList.hpp"

#include <sstream>


using namespace geng;


FrameList::FrameList(LayerContext &scene) : scene(scene) {
}

void FrameList::add_tables(const std::vector<FrameTable>& frameTables) {
    // Links each Frame Table to a Table ID.
    for (auto& i: frameTables) {
        tables.emplace_back(i);
    }
}

FrameTable& FrameList::get_table(int id) {
    if (id >= 0 && id < tables.size())
        return tables[id];
    std::stringstream ss;
    ss << "Table with table id " << id << " not found";
    scene.log(2, ss.str(), "FrameList::get_table");
    abort();
}

auto FrameList::begin() {
    return tables.begin();
}

auto FrameList::end() {
    return tables.end();
}
