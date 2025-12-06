#include "engine/effects/EffectManager.hpp"

using namespace gengine;

void EffectManager::add_effect(Object &o, Effect *e) {
    e->set_transform(o.t);
    effects.add(e);
}

void EffectManager::add_effect(Transform &t, Effect *e) {
    e->set_transform(t);
    effects.add(e);
}

void EffectManager::remove_effect(Object &o) {
    int target_id = o.id;
    for (auto it = effects.begin(); it != effects.end(); ++it)
        if ((*it) != nullptr && (*it)->get_target_id() == target_id)
            effects.erase(*it);
}


void EffectManager::remove_effect(Effect *e) {
    effects.erase(e);
}

void EffectManager::update() {
    for (auto & e : effects)
        if (e->update())
            remove_effect(e);
}