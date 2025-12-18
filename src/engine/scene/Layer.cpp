#include "engine/scene/Layer.hpp"

#include <utility>

using namespace geng;

Layer::Layer(std::string name)
        : world(nullptr), scene(std::move(name)), texreg(scene), actor(scene),
        event(scene), morph(scene),
        particle(scene), route(scene),
        init(texreg, scene) {
}

Layer::~Layer() {
    delete background;
}

void Layer::update(double dt) {
    // First, we update with a new scene dt.
    scene._update(dt);
    // Then we update all our objects
    std::vector<AnimInfo*> anim_infos = actor.update_objects();
    // Next we update all the frames of our objects
    frame.update(anim_infos);
    // Now we update the partices.
    particle.update();
    // Next we update all the routing info for our objects
    route.update();
    // Finally we update all of the morphs
    morph.update();
    // And we're done.
}

// *********************** //
// Rendering //
// *********************** //

std::vector<DrawBatch> Layer::_render_self(RenderBuffer& buffer) {
    if (gears.empty()) return {};
    // veector of batches for the renderer to handle
    std::vector<DrawBatch> batches;
    batches.reserve(texreg.size());
    if (texreg.size() == 0) {
        scene.log(2, "No textures detected to render layer "+ scene.get_name(), "Layer::_render_self()");
    }
    DrawBatch current = {texreg[0].texture, 0}; // Start with texreg[0] for particle fallback
    buffer.set_img_info(texreg[0].info);
    current.texture_id = 0;

    for (auto & gear : gears) {
        auto& g = *gear;

        if (g.texture_id != -1 && g.texture_id != current.texture_id) {
            current.num_vertices = buffer.size() - current.start_index;
            batches.push_back(current);

            current.start_index = buffer.size();
            current.texture_id = g.texture_id;
            current.tex = texreg[g.texture_id].texture;
            buffer.set_img_info(texreg[g.texture_id].info);
        }
        g.to_vertex(buffer);
    }

    current.num_vertices = buffer.size() - current.start_index;
    batches.push_back(current);
    return batches;
}

void Layer::_order() {
    // If the Layer is z-indexed
    if (scene.is_z_indexed()) {
        std::sort(gears.begin(), gears.end(), [](const Gear* e1, const Gear* e2) {
            return e1->z_index() < e2->z_index();
        });
    }
}

void Layer::_add_engine_context(EngineContext *ec) {
    world = ec;
    input._add_engine_context(ec);
    scene._add_engine_context(ec);
}

SDL_Texture * Layer::_find_texture(int texture_id) {
    return texreg.get_texture(texture_id).texture;
}

// ********************** //
// Private Functions //
// ********************** //
int Layer::pop_id() {
    if (!id_stack.is_empty())
        return id_stack.pop();
    return top_id++;
}

void Layer::add_gear(Gear *g) {
    int id = pop_id();
    g->id = id;

    if (!scene.is_batched())
        gears.push_back(g);
    else {
        auto it = std::upper_bound(
            gears.begin(), gears.end(), g->texture_id,
                [](int id, const Gear* g) { return id < g->texture_id; }
        );

        gears.insert(it, g);
    }
}

void Layer::remove_gear(Gear* g) {
    for (auto it = gears.begin(); it != gears.end(); ++it) {
        if (*it == g) {
            gears.erase(it);
            break;
        }
    }
}


// ********************** //
// Actors //
// ********************** //
void Layer::add_actor(Actor *a) {
    // First, we assign an ID and a type
    a->_engine_flagger(GFlag::actor);
    // Adds a gear to the engine
    add_gear(a);
    // Add object to manager
    actor.add_actor(a);
    // Applies the framestate
    frame.apply_framestate(*a);
}

void Layer::add_actors(const std::vector<Actor*>& actors) {
    for (auto& a: actors) {
        a->_engine_flagger(GFlag::actor);
        add_gear(a);
    }
    frame.apply_framestates(actors);
    actor.add_actors(actors);
}

void Layer::remove_actor(const Actor* a) {
    if (a == nullptr) return;
    particle.dissolve(a);
    morph.strip_morph(a);
    actor.dissolve(a);
    gears.erase(
    std::remove_if(
            gears.begin(),
            gears.end(),
            [&](const Gear* e){
                return e == a;
            }
        ),
        gears.end()
    );
}

void Layer::remove_actors(const std::vector<Actor*>& objs) {
    for (const auto& i: objs) {
        remove_actor(i);
    }
}

// ********************** //
// Particles //
// ********************** //
void Layer::instantiate_particle(ParticleGroup *pg) {
    // Define our element
    pg->_engine_flagger(GFlag::particle);
    // Add to our particle manager
    particle.add(pg);
    // Add it as a gear.
    add_gear(pg);
}

void Layer::attach_particle(Actor *o, ParticleGroup *pg) {
    instantiate_particle(pg);
    pg->horse = o;
}

void Layer::attach_particles(std::vector<ParticleGroup*>& pgs) {
    for (auto&i : pgs)
        instantiate_particle(i);
}

void Layer::detach_particle(Actor *a) {
    particle.dissolve(a);
}

void Layer::detach_particle(ParticleGroup* pg) {
    if (pg == nullptr) return;
    particle.remove(pg);
    remove_gear(pg);
}

void Layer::detach_particles(const std::vector<Actor*>& pg) {
    for (const auto& i: pg)
        detach_particle(i);
}

// ********************** //
// Effects //
// ********************** //
void Layer::apply_morph(geng::Morph *e) {
    morph.apply_morph(e);
}

void Layer::strip_morph(Gear *g) {
    morph.strip_morph(g);
}

void Layer::strip_morph(geng::Morph *e) {
    morph.strip_morph(e);
}

bool Layer::has_morph(Gear* g) {
    return morph.has_morph(g);
}

// *********************** //
// Routes //
// *********************** //
void Layer::set_route(Route *p) {
    route.add_path(p);
}

void Layer::set_route(Route *r, Gear* g, const Vertex &offset) {
    route.add_path(r, g, offset);
}

// *********************** //
// Initialization //
// *********************** //

TextureRegister& Layer::_init() {
    init._compose(frame, fl);
    // The renderer will emplace the correct textures into the texture register.
    return texreg;
}

// *********************** //
// Enums Boilerplate //
// *********************** //


void Layer::set_render_strict() {
    scene._strict();
}

void Layer::set_render_z_index() {
    scene._z_index();
}

void Layer::set_render_batch() {
    scene._batch();
    // Then we have to sort based on texture id.
    std::sort(gears.begin(), gears.end(), [](const Gear* a, const Gear* b) {
        return a->texture_id < b->texture_id;
    });
}






