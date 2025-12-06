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
    pg->horse = &o->t;
    add_particle(pg);
}

void Engine::remove_attached_particle(Object *o) {
    pm.dissolve(o);
}



void Engine::remove_object(const Object* o) {
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

void Engine::remove_objects(const std::vector<Object*>& objs) {
    for (const auto& i: objs) {
        remove_object(i);
    }
}

void Engine::remove_particle(ParticleGroup* pg) {
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

void Engine::remove_particles(const std::vector<ParticleGroup*>& pg) {
    for (const auto& i: pg)
        remove_particle(i);
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