#include "engine/animation/asset-info/FrameTableRegistry.hpp"

using namespace geng;

/**
* @return Returns the global FrameTableRegistry. Register a FrameTable with the syntax, @code static FrameTableRegister my_asset_name(ASSET_ID, FrameTable Object)@endcode
     */
FrameTableRegistry& geng::get_TableRegistry() {
    static FrameTableRegistry registry;
    return registry;
}

TableRegister::TableRegister(int table_id, const FrameTable &sheet) {
    std::cerr << "creating a table?\n";
    get_TableRegistry().emplace(table_id, sheet);
}
