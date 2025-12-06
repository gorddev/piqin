#include "../../include/engine/Engine.hpp"
#include "engine/gengine-globals/Timer.hpp"
#include "engine/textures/asset-info/SheetRegistry.hpp"
#include "engine/textures/asset-info/TextureRegister.hpp"

using namespace gengine;

// ReSharper disable once CppMemberInitializersOrder
Engine::Engine () :
        cam(0,0,Z_MAX, glb::scene.width, glb::scene.height),
        rm(&cam), sm(), om(), pm(), input(nullptr) {
}
Engine::~Engine () {}

void Engine::initialize() {
    rm.initialize();
    sm.initialize(rm.get_renderer());
    rm.set_texture_atlas(textures::tex_register[0]);
}

int Engine::pop_id() {
    if (!id_stack.is_empty())
        return id_stack.pop();
    return top_id++;
}

bool Engine::tick(double time) {
    // First we update our global scene
    glb::scene.update(time);

    // Now we check for user input
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return false;
        if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
            input.update(e);
    }

    GENG_Events.update();

    return true;
}

void Engine::render() {
    // Update all of our objects and their attatchments
    std::vector<FrameState*> frameStates = om.update_objects();
    sm.update(frameStates);
    // Render
    pm.update();
    if (pm.particles_to_remove())
        remove(pm.pop_removed_particles());

    std::sort(elements.begin(), elements.end(), [](const EngineElement& e1, const EngineElement& e2) {
        return *e1.z_index < *e2.z_index;
    });

    rm.render(elements);
    // Present our hard work
    rm.present();
}

void Engine::set_input_target(InputTarget *t) {
    input.setInputTarget(t);
}

void Engine::attach_new_particle(Object *o, ParticleGroup *pg) {
    pg->horse = o;
    add(pg);
}

void Engine::remove_attached_particle(Object *o) {
    pm.dissolve(o);
}

void Engine::add(Object *o) {
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

void Engine::add(std::vector<Object*> objs, GENG_Sort sort) {

    for (auto&i : objs) {
        i->id = pop_id();
        elements.emplace_back(GENG_Type::OBJECT, i->z(), i->id, i);
    }
    sm.apply_framestates(objs);
    om.add_objects(objs);
    return;

    // below is unused code
    // Now if we provided a descending order, we just switch
    if (sort == GENG_Sort::DESCENDING || sort == GENG_Sort::DESCENDING_BATCH)
        std::reverse(objs.begin(), objs.end());

    /* . ............    */
    // BATCH SORT
    // If we know our z-indexes are self-contained
    if (sort == GENG_Sort::ASCENDING_BATCH || sort == GENG_Sort::DESCENDING_BATCH) {
        // Convert to engine elmeents
        std::vector<EngineElement> batch;
        for (int i = 0; i < objs.size(); i++) {
            objs[i]->id = pop_id();
            batch.push_back(EngineElement(GENG_Type::OBJECT, objs[i]->z(), objs[i]->id, objs[i]));
        }
        // Find where we can insert.
        auto pos = std::lower_bound(
            elements.begin(), //d
            elements.end(),
            batch.front(),
            [](const EngineElement& a, const EngineElement& b) {
                return a.z_index < b.z_index;
            }
        );
        // Now we can insert our objects
        elements.insert(pos, batch.begin(), batch.end());
    }
    /* . ............    */
    // Otherwise we try and do it a little easier, but we don't have the luxury of pre-sort
    // JUST VECTOR SORT IN
    else if (sort == GENG_Sort::ASCENDING || sort == GENG_Sort::DESCENDING) {
        auto rit = elements.begin();
        auto oit = objs.begin();
        // Now we sort sort sort
        std::vector<EngineElement> newVec;
        newVec.reserve(objs.size() + elements.size());
        while (oit != objs.end() && rit != elements.end()) {
            if ((*oit)->z() < *rit->z_index) {
                Object *o = *oit;
                o->id = pop_id();
                newVec.emplace_back(GENG_Type::OBJECT, o->z(), o->id, reinterpret_cast<void*>(o));
                ++oit;
            }
            else
                newVec.push_back(*rit++);
        }
        newVec.insert(newVec.end(), rit, elements.end());

        // now update the z-sorter
        elements = newVec;
    }
    // Adds the objects to our object manager.
    om.add_objects(objs);
}

ParticleGroup *Engine::add(ParticleGroup *pg) {
    // First, we assign an ID
    pg->id = pop_id();
    // Then we create an engineElement
    EngineElement e(GENG_Type::PARTICLE, pg->pos.z, pg->id, pg);

    // Then we add the element according to it's z-pos.
    auto pos = std::lower_bound(
        elements.begin(), elements.end(),e, [](const EngineElement& e1, const EngineElement& e2) {
            return e1.z_index < e2.z_index;
        });
    // Now it's z-sorted!
    pm.add(pg);
    elements.insert(pos, e);
    return pg;
}

void Engine::add(std::vector<ParticleGroup*>& pgs, GENG_Sort sort) {
    if (sort != GENG_Sort::ASCENDING_BATCH && sort != GENG_Sort::DESCENDING_BATCH) {
        for (auto& i: pgs)
            add(i);
        return;
    }

    // Add all our particles
    pm.add(pgs);

    // Reverse if needed.
    if (sort == GENG_Sort::DESCENDING_BATCH)
        std::reverse(pgs.begin(), pgs.end());

    // Convert to engine elmeents
    std::vector<EngineElement> batch;
    for (int i = 0; i < pgs.size(); i++) {
        pgs[i]->id = pop_id();
        batch.push_back(EngineElement(GENG_Type::PARTICLE, pgs[i]->pos.z, pgs[i]->id, pgs[i]));
    }
    // compare to find insertion point
    auto pos = std::lower_bound(
        elements.begin(), //d
        elements.end(),
        batch.front(),
        [](const EngineElement& a, const EngineElement& b) {
            return a.z_index < b.z_index;
        }
    );

    // Now we insert our e
    elements.insert(pos, batch.begin(), batch.end());
}

void Engine::remove(const Object* o) {
    if (o == nullptr) return;
    om.dissolve(o);
    elements.erase(
    std::remove_if(
            elements.begin(),
            elements.end(),
            [&](const EngineElement& e){
                if (e.target == o) {
                    id_stack.push(&e - &elements[0]); // optional
                    return true;
                }
                return false;
            }
        ),
        elements.end()
    );
}

void Engine::remove(const std::vector<Object*>& objs) {
    for (const auto& i: objs) {
        remove(i);
    }
}

void Engine::remove(ParticleGroup* pg) {
    if (pg == nullptr) return;
    pm.remove(pg);
    elements.erase(
    std::remove_if(
            elements.begin(),
            elements.end(),
            [&](const EngineElement& e){
                if (e.target == pg) {
                    id_stack.push(&e - &elements[0]); // optional
                    return true;
                }
                return false;
            }
        ),
        elements.end()
    );
}

void Engine::remove(const std::vector<ParticleGroup*>& pg) {
    for (const auto& i: pg)
        remove(i);
}

void Engine::remove(const std::vector<int> ids) {
    for (auto& id: ids) {
        elements.erase(
        std::remove_if(
                elements.begin(),
                elements.end(),
                [&](const EngineElement& e){
                    if (e.id == id) {
                        id_stack.push(e.id); // optional
                        return true;
                    }
                    return false;
                }
            ),
            elements.end()
        );
    }
}