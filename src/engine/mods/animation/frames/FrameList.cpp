#include "engine/mods/animation/frames/FrameList.hpp"

#include "engine/debug/geng_debug.hpp"


using namespace gan;

int FrameList::add_table(FrameTable table) {
    tables.push_back(new FrameTable(table));
    return static_cast<int>(tables.size()) - 1;
}

void FrameList::add_tables(std::vector<FrameTable>& frameTables) {
    // Links each Frame Table to a Table ID.
    for (auto& i: frameTables) {
        add_table(std::move(i));
    }
}

FrameTable& FrameList::get_table(int id) {
    if (id >= 0 && id < static_cast<int>(tables.size()))
        return *tables[id];
    glog::err.src("FrameList::get_table") << "Table with table id " << id << " not found." << glog::endlog;
    return *tables[0];
}

auto FrameList::begin() {
    return tables.begin();
}

auto FrameList::end() {
    return tables.end();
}
