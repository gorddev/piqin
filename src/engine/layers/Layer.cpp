#include "engine/layers/Layer.hpp"

#include <utility>

using namespace geng;

Layer::Layer(std::string name)
        : engine_context(nullptr), scene(std::move(name)), texreg(scene), sprite(scene),
        event(scene), morph(scene),
        particle(scene), route(scene),
        init(texreg, scene), banner(scene),
        frame(scene), world(scene.camera),
        input(scene), physics(scene, world) {
    scene.log(1, "Layer created: " + scene.get_name(), "Layer::Layer");
}

Layer::~Layer() {
    delete background;
}

void Layer::update(double dt) {
    // We first update the scene with dt.
    scene._update(dt);
    // Then we update all our objects' positions
    sprite.update();
    // We manage the physics for sprites
    physics.update();
    // Then we update the spritesheet of each sprite
    sprite.update_sprites();
    // Now we update the partices.
    particle.update();
    // Next we update any dynamic banner elements
    banner.update();
    // Next we update all the routing info for our objects
    route.update();
    // Next we update all the morphs
    morph.update();

}

// *********************** //
// Rendering //
// *********************** //

std::vector<DrawBatch> Layer::_render_self(RenderBuffer& buffer) {
    // If we have no gears, we return
    if (gears.empty()) return {};

    // Then we set up the camera position
    buffer.set_camera_pos({scene.camera.pos.x, scene.camera.pos.y});

    // If we're z-ordered, we sort based on that
    std::sort(gears.begin(), gears.end(), [](const Gear* e1, const Gear* e2) {
        return e1->z_index < e2->z_index;
    });

    // If the debug grid is enabled
    if (active && scene.world->debugger.is_grid())
        _render_grid(buffer, 16, 1, true);

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

    // We render our world here
    if (world.get_texture_id() != -1) {
        current_batch.texture_id = world.get_texture_id();
        buffer.set_img_info(texreg[world.get_texture_id()].info);
        current_batch.tex = texreg[world.get_texture_id()].texture;
    }
    world.render_world(buffer);

    // We render our gears here.
    _render_gears(buffer, current_batch, batches);

    // Holy we're done!
    // Wait what about debug mode
    if (active&& scene.world->debugger.is_hitboxes())
        _render_hitboxes(buffer);

    // We push back the final batch before exiting the render self function
    current_batch.num_vertices = buffer.size() - current_batch.start_index;
    batches.push_back(current_batch);

    return batches;
}

void Layer::_render_gears(RenderBuffer& buffer, DrawBatch& current_batch, std::vector<DrawBatch>& batches) {
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
}

void Layer::_render_grid(RenderBuffer& buffer, int grid_div, int thickness, bool subdiv) const {
    Pos2D startpos = {scene.camera.pos.x - scene.camera.pos.x%grid_div, scene.camera.pos.y - scene.camera.pos.y%grid_div};
    Pos2D endpos = {};
    for (int i = startpos.x; i < scene.camera.right(); i+= grid_div) {
        Box2D line = {i, scene.camera.pos.y, thickness, scene.camera.get_height()};
        std::vector<SDL_FPoint> points;
        line.to_vert_points(points);
        for (auto& i: points)
            buffer.push_back(i, {255, 255, 255, 90});
    }
    for (int i = startpos.y; i < scene.camera.bottom(); i+= grid_div) {
        Box2D line = {scene.camera.pos.x, i, scene.camera.get_width(), thickness};
        std::vector<SDL_FPoint> points;
        line.to_vert_points(points);
        for (auto& i: points)
            buffer.push_back(i, {255, 255, 255, 40});
    }

    if (subdiv) {
        for (int i = startpos.x + grid_div/2; i < scene.camera.right(); i+= grid_div) {
            Box2D line = {i, scene.camera.pos.y, thickness, scene.camera.get_height()};
            std::vector<SDL_FPoint> points;
            line.to_vert_points(points);
            for (auto& i: points)
                buffer.push_back(i, {255, 255, 255, 20});
        }
        for (int i = startpos.y + grid_div/2; i < scene.camera.bottom(); i+= grid_div) {
            Box2D line = {scene.camera.pos.x, i, scene.camera.get_width(), thickness};
            std::vector<SDL_FPoint> points;
            line.to_vert_points(points);
            for (auto& i: points)
                buffer.push_back(i, {255, 255, 255, 20});
        }
    }
}

void Layer::_render_hitboxes(RenderBuffer& buffer) {
    // we need to go through and render all the hitboxes
    for (auto & gear : gears) {
        auto& g = *gear;
        std::vector<SDL_FPoint> hitbox = g.t.to_vertex_hitbox(1);
        if (g.is_banner())
            buffer.push_back(hitbox, {0, 255, 0, 255});
        else if (g.is_sprite())
            buffer.push_back(hitbox, {0, 255, 255, 255});
        else
            buffer.push_back(hitbox, {0, 0, 255, 255});
    }
    // goes through the physics scene
    physics.render_hitboxes(buffer, {255, 0, 0, 255}, 1);
    // goes through all the colliders in the physics engine
    for (auto & c : physics.get_colliders()) {
        // first we actually render the physical scene
        std::vector<SDL_FPoint> hitbox = c->to_vertex_hitbox(1);
        buffer.push_back(hitbox, {255, 0, 0, 255});
    }
}

void Layer::_order() {
    // If the Layer is z-indexed
    if (scene.is_z_indexed()) {
        std::sort(gears.begin(), gears.end(), [](const Gear* e1, const Gear* e2) {
            return e1->z_index < e2->z_index;
        });
    }
}

void Layer::_add_engine_context(EngineContext *ec) {
    engine_context = ec;
    scene._add_engine_context(ec);
}

SDL_Texture * Layer::_find_texture(int texture_id) {
    return texreg.get_texture(texture_id).texture;
}

void Layer::_set_active(bool status) {
    std::cerr << scene.get_name() + " active: " << status << std::endl;
    active = status;
}

bool Layer::is_active() const {
    return active;
}

// ********************** //
// Private Functions //
// ********************** //

bool Layer::add_gear(Gear *g) {
    if (g->id == -1)
        g->id = id;
    else {
        scene.log(1, "Cannot add item, as it has already been assigned to a layer.", "Layer::add_gear");
        return false;
    }

    if (!scene.is_batched())
        gears.push_back(g);
    else {
        auto it = std::upper_bound(
            gears.begin(), gears.end(), g->texture_id,
                [](int id, const Gear* g) { return id < g->texture_id; }
        );
        gears.insert(it, g);
    }
    return true;
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
// Sprites //
// ********************** //
void Layer::add_sprite(Sprite *a) {
    // First, we assign an ID and a type
    a->_engine_flagger(GFlag::sprite);
    // Adds a gear to the engine
    add_gear(a);
    // Add object to manager
    sprite.add_sprite(a);
}

void Layer::add_sprites(const std::vector<Sprite*>& sprites) {
    for (auto& a: sprites) {
        a->_engine_flagger(GFlag::sprite);
        add_gear(a);
    }
    sprite.add_sprites(sprites);
}

void Layer::remove_sprite(Sprite* a) {
    if (a == nullptr) return;
    particle.dissolve(a);
    morph.strip_morph(a);
    sprite.dissolve(a);
    remove_gear(a);
}

void Layer::remove_sprites(const std::vector<Sprite*>& objs) {
    for (const auto& i: objs) {
        remove_sprite(i);
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
    if (add_gear(pg)) {
        // Define our element
        pg->_engine_flagger(GFlag::particle);
        // Add to our particle manager
        particle.add(pg);
        // Add it as a gear.
    }
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

void Layer::detach_particle(Sprite *a) {
    particle.dissolve(a);
}

void Layer::detach_particle(ParticleGroup* pg) {
    if (pg == nullptr) return;
    if (pg->id != id)
        scene.log(1, "Item does not belong to layer.", "Layer::detach_particle()");
    else {
        particle.remove(pg);
        remove_gear(pg);
    }
}

void Layer::detach_particles(const std::vector<Sprite*>& pg) {
    for (const auto& i: pg)
        detach_particle(i);
}

// ********************** //
// Effects //
// ********************** //
void Layer::apply_morph(geng::Morph *e) {
    if (e->get_gear_id() != id)
        scene.log(1, "Item does not belong to layer.", "Layer::apply_morph()");
    else
        morph.apply_morph(e);
}

void Layer::strip_morph(Gear *g) {
    if (g->id != id)
        scene.log(1, "Item does not belong to layer.", "Layer::strip_morph");
    else
        morph.strip_morph(g);
}

void Layer::link_collider(Collider *c) {
    if (c->id != -1) {
        scene.log(1, "Cannot add collider, as it has already been assigned a layer.", "Layer::link_collider()");
        physics.remove_collider(c);
    }
    else
        add_sprite(c);
    physics.add_collider(c);
}

void Layer::unlink_collider(Collider *c) {
    physics.remove_collider(c);
}

void Layer::strip_morph(geng::Morph *e) {
    if (e->get_gear_id() != id)
        scene.log(1, "Item does not belong to layer.", "Layer::strip_morph");
    else
        morph.strip_morph(e);
}

bool Layer::has_morph(Gear* g) {
    if (g->id != id)
        scene.log(1, "Item does not belong to layer.", "Layer::has_morph");
    else
        return morph.has_morph(g);
    return false;
}

// *********************** //
// Routes //
// *********************** //
void Layer::set_route(Route *p) {
    if (!engine_context)
        scene.log(1, "Cannot add route, as layer is not composed yet.", "Layer::set_route");
    else if (p->get_payload()->id != id)
        scene.log(1, "Cannot add route, as payload does not belong to layer.", "Layer::set_route");
    route.add_route(p);
}

void Layer::set_route(Route *r, Gear* g, const FPos2D &offset) {
    if (!engine_context)
        scene.log(1, "Cannot add route, as layer is not composed yet.", "Layer::set_route");
    else if (r->get_payload()->id != id)
        scene.log(1, "Cannot add route, as payload does not belong to layer.", "Layer::set_route");
    route.add_route(r, g, offset);
}

// *********************** //
// Initialization //
// *********************** //

TextureRegister& Layer::_init() {
    init._compose(frame, fl, tilesets);
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

void Layer::load_world(std::string filename, int tileset_num) {
    if (engine_context == nullptr) {
        std::cerr << "[FATAL] Cannot load world without tileset loaded. Call engine.compose_layer() before loading a world." << std::endl;
        abort();
    }
    world.load_world(std::move(filename), &tilesets[tileset_num]);
    physics.load_current_level();
}

Tileset & Layer::get_tileset(int index) {
    return tilesets[index];
}

FPos2D Layer::get_scene_center() const {
    if (engine_context != nullptr)
        return {engine_context->get_width()/2.f, engine_context->get_height()/2.f};
    return {-1, -1};
}






