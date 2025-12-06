#include "engine/Engine.hpp"

using namespace gengine;

void Engine::add_object(Object *o) {
    // First, we assign an ID
    Object& obj = *o;
    obj.id = pop_id();
    // Add object to manager
    om.add_object(o);
    sm.apply_framestate(obj);
    // Then we create an engineElement
    EngineElement e(GENG_Type::OBJECT, obj.z(), obj.id, o);

    // Then we add the element according to it's z-pos.
    const auto pos = std::lower_bound(
        elements.begin(), elements.end(),e, [](const EngineElement& e1, const EngineElement& e2) {
            return *e1.z_index < *e2.z_index;
        });

    // Now it's z-sorted!
    elements.insert(pos, e);
}

void Engine::add_objects(std::vector<Object*> objs) {

    for (auto&i : objs) {
        i->id = pop_id();
        elements.emplace_back(GENG_Type::OBJECT, i->z(), i->id, i);
    }
    sm.apply_framestates(objs);
    om.add_objects(objs);
}

void Engine::add_particle(ParticleGroup *pg) {
    // First, we assign an ID
    pg->id = pop_id();
    // Then we create an engineElement
    EngineElement e(GENG_Type::PARTICLE, pg->pos.z, pg->id, pg);
    // Add to our particle manager
    pm.add(pg);
    // Add to our engine
    elements.push_back(e);
}

void Engine::add_particle(Object *o, ParticleGroup *pg) {
    pm.add(pg);
    pg->horse = &o->t;
}

void Engine::add_particles(std::vector<ParticleGroup*>& pgs) {
    // Add all our particles
    pm.add(pgs);
    // Convert to engine elmeents
    std::vector<EngineElement> batch;
    for (int i = 0; i < pgs.size(); i++) {
        pgs[i]->id = pop_id();
        batch.push_back(EngineElement(GENG_Type::PARTICLE, pgs[i]->pos.z, pgs[i]->id, pgs[i]));
    }
    // Now we insert our engine elements
    elements.insert(elements.end(), batch.begin(), batch.end());
}

void Engine::add_effect(Object& o, Effect *e) {
    em.add_effect(o, e);
}

void Engine::add_effect(Transform &t, Effect *e) {
    em.add_effect(t, e);
}

