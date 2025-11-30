#include "../../include/engine/Engine.hpp"

#include "particles/ParticleManager.hpp"

using namespace gengine;

Engine::Engine () :
        cam(0,0,Z_MAX, scene::width, scene::height),
        rm(&cam), sm(), om(), pm(), input(nullptr) {
    // Now we initialize each of our objects.
    rm.initialize();									// Renderer
    sm.initialize(rm.get_renderer());		// SheetManager
}
Engine::~Engine () {}

int Engine::pop_id() {
    if (!id_stack.is_empty())
        return id_stack.pop();
    return top_id++;
}

void Engine::tick() {
    // Update all of our objects and their attatchments
    std::vector<FrameState *> frameStates = om.update_objects();
    // Tick all the frames of all objects/other shit
    sm.tick_frames(frameStates);
    // Render
    rm.render();
    // Present our hard work
    rm.present();
}

void Engine::add(Object *o) {
    // First, we assign an ID
    o->id = pop_id();
    // Add object to manager
    om.add_object(o);
    // Then we create an engineElement
    EngineElement e(Type::OBJECT, o->z(), o->id, o);

    // Then we add the element according to it's z-pos.
    auto pos = std::lower_bound(
        renderTargets.begin(), renderTargets.end(),e, [](const EngineElement& e1, const EngineElement& e2) {
            return e1.z_index < e2.z_index;
        });
    // Now it's z-sorted!
    renderTargets.insert(pos, e);

}

void Engine::add(std::vector<Object*>& objs, Sort sort) {
    // Now if we provided a descending order, we just switch
    if (sort == Sort::DESCENDING || sort == Sort::DESCENDING_BATCH)
        std::reverse(objs.begin(), objs.end());

    /* . ............    */
    // BATCH SORT
    // If we know our z-indexes are self-contained
    if (sort == Sort::ASCENDING_BATCH || sort == Sort::DESCENDING_BATCH) {
        // Convert to engine elmeents
        std::vector<EngineElement> batch;
        for (int i = 0; i < objs.size(); i++) {
            objs[i]->id = pop_id();
            batch.push_back(EngineElement(Type::OBJECT, objs[i]->z(), objs[i]->id, objs[i]));
        }
        // Find where we can insert.
        auto pos = std::lower_bound(
            renderTargets.begin(), //d
            renderTargets.end(),
            batch.front(),
            [](const EngineElement& a, const EngineElement& b) {
                return a.z_index < b.z_index;
            }
        );
        // Now we can insert our objects
        renderTargets.insert(pos, batch.begin(), batch.end());
    }
    /* . ............    */
    // Otherwise we try and do it a little easier, but we don't have the luxury of pre-sort
    // JUST VECTOR SORT IN
    else if (sort == Sort::ASCENDING || sort == Sort::DESCENDING) {
        auto rit = renderTargets.begin();
        auto oit = objs.begin();
        // Now we sort sort sort
        std::vector<EngineElement> newVec;
        newVec.reserve(objs.size() + renderTargets.size());
        while (oit != objs.end() && rit != renderTargets.end()) {
            if ((*oit)->z() < rit->z_index) {
                Object *o = *oit;
                o->id = pop_id();
                newVec.emplace_back(Type::OBJECT, o->z(), o->id, reinterpret_cast<void*>(o));
                ++oit;
            }
            else
                newVec.push_back(*rit++);
        }
        newVec.insert(newVec.end(), rit, renderTargets.end());

        // now update the z-sorter
        renderTargets = newVec;
    }
    // Adds the objects to our object manager.
    om.add_objects(objs);
}

void Engine::add(ParticleGroup *pg) {
    // First, we assign an ID
    pg->id = pop_id();
    // Then we create an engineElement
    EngineElement e(Type::PARTICLE, pg->z(), pg->id, pg);

    // Then we add the element according to it's z-pos.
    auto pos = std::lower_bound(
        renderTargets.begin(), renderTargets.end(),e, [](const EngineElement& e1, const EngineElement& e2) {
            return e1.z_index < e2.z_index;
        });
    // Now it's z-sorted!
    pm.add(pg);
    renderTargets.insert(pos, e);
}

void Engine::add(std::vector<ParticleGroup*>& pgs, Sort sort) {
    if (sort != Sort::ASCENDING_BATCH && sort != Sort::DESCENDING_BATCH) {
        for (auto& i: pgs)
            add(i);
        return;
    }

    // Add all our particles
    pm.add(pgs);

    // Reverse if needed.
    if (sort == Sort::DESCENDING_BATCH)
        std::reverse(pgs.begin(), pgs.end());

    // Convert to engine elmeents
    std::vector<EngineElement> batch;
    for (int i = 0; i < pgs.size(); i++) {
        pgs[i]->id = pop_id();
        batch.push_back(EngineElement(Type::PARTICLE, pgs[i]->pos.z, pgs[i]->id, pgs[i]));
    }
    // compare to find insertion point
    auto pos = std::lower_bound(
        renderTargets.begin(), //d
        renderTargets.end(),
        batch.front(),
        [](const EngineElement& a, const EngineElement& b) {
            return a.z_index < b.z_index;
        }
    );

    // Now we insert our e
    renderTargets.insert(pos, batch.begin(), batch.end());
}

void Engine::remove(Object* o) {
    om.dissolve(o);
    renderTargets.erase(
    std::remove_if(
            renderTargets.begin(),
            renderTargets.end(),
            [&](const EngineElement& e){
                if (e.target == o) {
                    id_stack.push(&e - &renderTargets[0]); // optional
                    return true;
                }
                return false;
            }
        ),
        renderTargets.end()
    );
}

void Engine::remove(std::vector<Object*>& objs) {
    for (auto& i: objs) {
        remove(i);
    }
}

void Engine::remove(ParticleGroup* pg) {
    pm.dissolve(pg);
    renderTargets.erase(
    std::remove_if(
            renderTargets.begin(),
            renderTargets.end(),
            [&](const EngineElement& e){
                if (e.target == pg) {
                    id_stack.push(&e - &renderTargets[0]); // optional
                    return true;
                }
                return false;
            }
        ),
        renderTargets.end()
    );
}

void Engine::remove(std::vector<ParticleGroup*>& pg) {
    for (auto& i: pg)
        remove(i);
}

void Engine::update_z(Object* o) {
    for (int i = 0; i < renderTargets.size(); i++) {
        if (renderTargets[i].target == o) {
            renderTargets.erase(renderTargets.begin() + i);
        }
    }
    add(o);
}
void Engine::update_z(std::vector<Object*>& objs) {
    for (auto& i: objs) {
        add(i);
    }
}