#pragma once
#include "engine/mods/plugins/trails/TrailModule.hpp"

using namespace gfx;

void TrailModule::on_gear_removal(gan::Gear *gear) {
    Module::on_gear_removal(gear);
}

TrailModule::TrailModule(const gan::ModuleInit init)
    : Module(init) {}

void TrailModule::add_trail(Trail *trail) {
    trails.push_back(trail);
}

void TrailModule::remove_trail(const Trail* trail) {
    for (size_t i = 0; i < trails.size(); i++) {
        if (trail == trails[i]) {
            std::swap(trails[i], trails.back());
            delete trails.back();
        }
    }
}

void TrailModule::remove_trail_from(const gan::Gear* g) {
    for (size_t i = 0; i < trails.size(); i++) {
        if (g == &trails[i]->payload) {
            std::swap(trails[i], trails.back());
            delete trails.back();
        }
    }
}

void TrailModule::update() {
    for (auto& trail : trails) {
        trail->update(scene.state);
    }
}

