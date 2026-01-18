#include "engine/scene/morphs/MorphManager.hpp"

using namespace gan;

MorphManager::MorphManager(LayerContext &layer_context) : scene(layer_context) {}

void MorphManager::apply_morph(Morph *e) {
    morphs.add(e);
}

void MorphManager::strip_morph(const Gear* g) {
    for (auto& e: morphs) {
        if (e != nullptr && e->get_gear_ptr() == g) {
            strip_morph(e);
            break;
        }
    }
}

void MorphManager::strip_morph(Morph *e) {
    morphs.erase(e);
    delete e;
}

void MorphManager::update() {
    for (auto it = morphs.begin(); it != morphs.end(); ++it) {
        auto m = *it;
        if (m != nullptr && m->update(scene.state))
            strip_morph(m);
    }
}

bool MorphManager::has_morph(const Gear *g) {
    return std::any_of(morphs.begin(),
        morphs.end(),
        [g](const Morph *m)
        { return m->get_gear_ptr() == g; });
}
