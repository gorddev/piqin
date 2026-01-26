#include "engine/mods/ModuleManager.hpp"

using namespace gan;

ModuleManager::ModuleManager(LayerCore &context)
    :context(context) {}

ModuleManager::~ModuleManager() {
    for (auto& m: order)
        delete m;
}

void ModuleManager::notify_gear_removal(Gear* g, uint_fast16_t module_id) {
    if (modules.contains(module_id))
        modules[module_id]->on_gear_removal(g);
    else
        glog::err.src("ModuleManager::notify_gear_removal()") << "Module with id " << module_id << "does not exist." << glog::endlog;
}

void ModuleManager::pre_update() {
    for (auto& m: order) {
        if (!(m->_update_ & Module::NO_PRE_UPDATE))
            m->update();
    }
}

void ModuleManager::update() {
    for (auto& m: order) {
        if (!(m->_update_ & Module::NO_UPDATE))
            m->update();
    }
}

void ModuleManager::post_update() {
    for (auto& m: order) {
        if (!(m->_update_ & Module::NO_POST_UPDATE))
            m->post_update();
    }
}