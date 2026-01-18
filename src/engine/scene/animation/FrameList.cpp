#include "engine/scene/animation/FrameList.hpp"

#include "engine/debug/geng_debug.hpp"


using namespace gan;

int FrameList::add_table(const FrameTable &table) {
    tables.push_back(table);
    return tables.size() - 1;
}

void FrameList::add_tables(gch::vector<FrameTable>& frameTables) {
    // Links each Frame Table to a Table ID.
    for (auto& i: frameTables) {
        tables.emplace_back(i);
    }
}

FrameTable& FrameList::get_table(int id) {
    if (id >= 0 && id < tables.size())
        return tables[id];
    glog::err.src("FrameList::get_table") << "Table with table id " << id << " not found." << glog::endlog;
}

auto FrameList::begin() {
    return tables.begin();
}

auto FrameList::end() {
    return tables.end();
}
