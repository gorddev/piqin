#include "../../../../include/engine/scene/layers/Layer.hpp"

#include <utility>

using namespace geng;

Layer::Layer(std::string name)
        : world(nullptr), scene(std::move(name)), texreg(scene), actor(scene),
        event(scene), morph(scene),
        particle(scene), route(scene),
        init(texreg, scene), banner(scene), frame(scene) {
    std::cerr << "whats going on\n";
    scene.log(1, "Layer created: " + scene.get_name(), "Layer::Layer");
}

Layer::~Layer() {
    delete background;
}

void Layer::update(double dt) {
    // We first update the scene with dt.
    scene._update(dt);
    // Then we update all our objects
    actor.update();
    // Now we update the partices.
    particle.update();
    // Next we update any dynamic banner elements
    banner.update();
    // Next we update all the routing info for our objects
    route.update();
    // Finally we update all of the morphs
    morph.update();
}

// *********************** //
// Rendering //
// *********************** //

std::vector<DrawBatch> Layer::_render_self(RenderBuffer& buffer) {
    // If we have no gears, we return
    if (gears.empty()) return {};

    // If we're z-ordered, we sort based on that
    std::sort(gears.begin(), gears.end(), [](const Gear* e1, const Gear* e2) {
        return e1->z_index() < e2->z_index();
    });

    // veector of batches for the renderer to handle
    std::vector<DrawBatch> batches;
    // in an ideal scenario, we reserve the number of textures we have
    batches.reserve(texreg.size());
    // If we have no textures, we crash.
    if (texreg.size() == 0)
        scene.log(2, "No textures detected to render layer "+ scene.get_name(), "Layer::_render_self()");
    // Create the current draw batch from the initial texture. (This will likely be overwritten)
    std::pair<int, Texture> initial = texreg.front();
    DrawBatch current_batch = {initial.second.texture, 0}; // Start with texreg[0] for particle fallback
    // Set the img_info for the RenderBuffer for various objects to use.
    buffer.set_img_info(initial.second.info);
    // Setrs the current texture id to 0.
    current_batch.texture_id = initial.first;

    for (auto & gear : gears) {
        // Reference our gear for faster access.
        auto& g = *gear;
        // If our texture ID doesn't match, we end the batch and create a new one (if -1, it doesn't have a texture, so it's chill)
        if (g.texture_id != -1 && g.texture_id != current_batch.texture_id) {
            // First we get the number of vertices we'll render
            current_batch.num_vertices = buffer.size() - current_batch.start_index;
            // Then we push back the current batch cause we won't be needing it anymore.
            batches.push_back(current_batch);
            // next, we get the start index for the next batch
            current_batch.start_index = buffer.size();
            // along with the requisite texture_id
            current_batch.texture_id = g.texture_id;
            // Then we set the texture
            current_batch.tex = texreg[g.texture_id].texture;
            // and update our image info.
            buffer.set_img_info(texreg[g.texture_id].info);
        }
        g.to_vertex(buffer);
    }

    // Holy we're done!
    // Wait what about debug mode
    if (scene.is_debug()) {
        // we need to go through and render all the hitboxes
        for (auto & gear : gears) {
            auto& g = *gear;
            std::vector<SDL_FPoint> hitbox = g.t.to_vertex_hitbox(1);
            if (g.is_banner())
                buffer.push_back(hitbox, {0, 255, 0, 255});
            else if (g.is_actor())
                buffer.push_back(hitbox, {255, 0, 0, 255});
            else
                buffer.push_back(hitbox, {0, 0, 255, 255});
        }
    }

    // We push back the final batch before exiting the render self function
    current_batch.num_vertices = buffer.size() - current_batch.start_index;
    batches.push_back(current_batch);

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
}

void Layer::add_actors(const std::vector<Actor*>& actors) {
    for (auto& a: actors) {
        a->_engine_flagger(GFlag::actor);
        add_gear(a);
    }
    actor.add_actors(actors);
}

void Layer::remove_actor(Actor* a) {
    if (a == nullptr) return;
    particle.dissolve(a);
    morph.strip_morph(a);
    actor.dissolve(a);
    remove_gear(a);
}

void Layer::remove_actors(const std::vector<Actor*>& objs) {
    for (const auto& i: objs) {
        remove_actor(i);
    }
}


// ********************** //
// Banners //
// ********************** //
void Layer::add_banner(Banner *b) {
    banner.add_banner(b);
    b->_engine_flagger(GFlag::banner);
    add_gear(b);
}

void Layer::add_banners(
    const std::vector<Banner *> &banners) {
    for (const auto& i: banners) {
        add_banner(i);
        add_gear(i);
    }
}

void Layer::remove_banner(Banner *b) {
    banner.remove_banner(b);
    remove_gear(b);
}

void Layer::remove_banners(const std::vector<Banner*>& banners) {
    for (const auto& i: banners) {
        remove_banner(i);
        remove_gear(i);
    }
}

Font & Layer::get_font(int index) {
    return fl[index];
}

FrameTable & Layer::get_frame_table(int index) {
    return frame.get_table(index);
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

void Layer::attach_particle(Gear *g, ParticleGroup *pg) {
    instantiate_particle(pg);
    pg->horse = g;
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

Vertex Layer::get_scene_center() const {
    return {world->get_width()/2.f, world->get_height()/2.f, 0};
}






