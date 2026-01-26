#include "engine/layers/layer-subobjects/GearList.hpp"

using namespace gan;


GearList::GearList(GearRemovalFunction bind, int& layer_id, void* module_manager)
    : module_manager(module_manager), gear_removal_function(bind), layer_id(layer_id)
    {}

Gear*& GearList::operator[](uint_fast32_t index) {
    if (index >= gears.size())
        glog::err.src("GearList::operator[]") << "Index " << index << "out of bounds." << glog::endlog;
    return gears[index];
}

// Destructor
GearList::~GearList() {
    for (auto& g: gears)
        delete g;
}



// <><><> Adding and removing <><><>
bool GearList::add_gear(Gear *g, const uint_fast16_t& module_id) {
    if (g->id != -1) {
        glog::warn.src("GearList::add_gear") << "Gear cannot be added to layer " << layer_id << " as it has already been assigned a layer." << glog::endlog;
        return false;
    }
    g->id = layer_id;
    // add to our gear to module map
    gear_map[g] = std::make_pair(module_id, gears.size());
    // the gear vector for iterating
    gears.push_back(g);
    return true;
}

// removal function with zero iteration.
void GearList::remove_gear(Gear *g) {
    // check if the gear actually exists in the map
    if (gear_map.contains(g)) {
        // call the gear removal function
        gear_removal_function(module_manager, g, gear_map[g].first);
        // update the back's index with the current module's index
        auto& back_index = gear_map[gears.back()].second;
        // update the recorded index with it's new position
        back_index = gear_map[g].second;
        // swap it to the new position
        std::swap(gears.back(), gears[back_index]);
        // pop the back
        gears.pop_back();
        // clear the map
        gear_map.erase(g);
    }


}

void GearList::sort_gears() {
    std::sort(gears.begin(), gears.end(), [](const Gear* e1, const Gear* e2) {
        return e1->z_index < e2->z_index;
    });
}

void GearList::render_gears(RenderBuffer &buffer) {
    for (auto & gear : gears) {
        // Reference our gear for faster access.
        auto& g = *gear;
        // If our texture ID doesn't match, we end the batch and create a new one (if -1, it doesn't have a texture, so it's chill)
        buffer.request_texture(g.texture_id);
        // Adds the gear's vertices to the buffer.
        buffer.begin_object();
        g.to_vertex(buffer);
        buffer.end_object();
    }
}

void GearList::render_hitboxes(RenderBuffer& buffer, uint16_t cam_width) const {
    // we need to go through and render all the hitboxes
    int thick = 1 + (cam_width / 400);
    for (auto & gear : gears) {
        auto& g = *gear;
        if (g.t.w == 0 || g.t.h == 0)
            continue;
        std::vector<SDL_FPoint> hitbox = g.t.to_vertex_hitbox(thick);
        buffer.begin_object();
        if (g.is_banner())
            buffer.push_back(hitbox, {0, gan::max_alpha, 0, gan::max_alpha});
        else if (g.is_sprite())
            buffer.push_back(hitbox, {0, gan::max_alpha, gan::max_alpha, gan::max_alpha});
        else
            buffer.push_back(hitbox, {0, 0, gan::max_alpha, gan::max_alpha});
        buffer.end_object();
    }
}




