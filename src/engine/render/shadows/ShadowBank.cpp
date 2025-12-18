#include "engine/rendering/shadows/ShadowBank.hpp"

#include <iostream>

using namespace geng;

ShadowBank::ShadowBank() {
    currentFunc = shadowfuncs["background"];
}

void ShadowBank::set_function(const std::string &shadow) {
    if (shadowfuncs.find(shadow) != shadowfuncs.end()) {
        currentFunc = shadowfuncs[shadow];
    }
    else {
        std::cerr << "FATAL: Shadow function does not exist.\n";
        abort();
    }
}
void ShadowBank::add_function(const std::string& shadow, shadowFunc func) {
    if (shadowfuncs.find(shadow) != shadowfuncs.end()) {
        std::cerr << "Err adding shadow: function already exists.\n";
    }
    else {
        shadowfuncs[shadow] = std::move(func);
    }
}

void ShadowBank::apply_shadow(std::vector<SDL_Vertex> &buffer, int numVertices, std::string shadow_type) {
    if (buffer.size() < numVertices) {
        std::cerr << "ERR: Requesting more shadows than Vertices rendered.\n";
        numVertices = buffer.size();
    }
    if (shadowfuncs.find(shadow_type) == shadowfuncs.end()) {
        std::cerr << "Shadow function does not exist.\n";
        return;
    }
    shadowfuncs[shadow_type](buffer, numVertices, nullptr);
}

void ShadowBank::apply_shadow(std::vector<SDL_Vertex>& buffer, int numVertices) {
    if (buffer.size() < numVertices) {
        std::cerr << "ERR: Requesting more shadows than Vertices rendered.\n";
        numVertices = buffer.size();
    }
    currentFunc(buffer, numVertices, nullptr);
}
void ShadowBank::set_floor(int shadowFloor) {
    floor = shadowFloor;
}