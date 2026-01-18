#include "engine/rendering/shadows/ShadowBank.hpp"

#include "../../../../include/engine/debug/geng_debug.hpp"

using namespace gan;

ShadowBank::ShadowBank() {
    currentFunc = shadowfuncs["background"];
}

void ShadowBank::set_function(const hstring shadow) {
    if (shadowfuncs.find(shadow) != shadowfuncs.end()) {
        currentFunc = shadowfuncs[shadow];
    }
    else {
        glog::err << "FATAL: Shadow function does not exist." << glog::endlog;
    }
}
void ShadowBank::add_function(const hstring shadow, shadowFunc func) {
    if (shadowfuncs.find(shadow) != shadowfuncs.end()) {
        glog::warn << "Err adding shadow: function already exists." << glog::endlog;
    }
    else {
        shadowfuncs[shadow] = std::move(func);
    }
}

void ShadowBank::apply_shadow(gch::vector<SDL_Vertex> &buffer, int numVertices, hstring shadow_type) {
    if (buffer.size() < numVertices) {
        glog::warn << "ERR: Requesting more shadows than Vertices rendered." << glog::endlog;
        numVertices = buffer.size();
    }
    if (shadowfuncs.find(shadow_type) == shadowfuncs.end()) {
        glog::warn << "Shadow function does not exist." << glog::endlog;
        return;
    }
    shadowfuncs[shadow_type](buffer, numVertices, nullptr);
}

void ShadowBank::apply_shadow(gch::vector<SDL_Vertex>& buffer, int numVertices) {
    if (buffer.size() < numVertices) {
        glog::err << "ERR: Requesting more shadows than Vertices rendered.\n" << glog::endlog;
        numVertices = buffer.size();
    }
    currentFunc(buffer, numVertices, nullptr);
}
void ShadowBank::set_floor(int shadowFloor) {
    floor = shadowFloor;
}