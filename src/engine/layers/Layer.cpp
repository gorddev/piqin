#include "engine/layers/Layer.hpp"

#include <utility>

using namespace geng;

Layer::Layer(LayerInit& layer_init, fstring<10> name)
        : core(layer_init.core), camera(layer_init.camera), texreg(layer_init.texreg),
        name(name), scene(this->name, layer_init.camera),
        sprites(scene), events(scene), morphs(scene),
        particles(scene), routes(scene),
        init(scene, frames, fonts, tiles, texreg), banners(),
        frames(), world(layer_init.camera),
        input(scene), physics(scene, world) {

}

Layer::~Layer() {
    delete background;
}

void Layer::update(double dt) {
    if (scene.is_running()) {
        // 1. Grab time from the main loop
        scene._update(dt);
        // 2. Update each sprites via. their update fcuntion
        sprites.update();
        // 3. Update the physics of each collider sprites
        physics.update();
        // 4. Update the spritesheet of each sprites after everything
        sprites.update_frames();
        // 5. Update particle effects
        particles.update();
        // 6. Update banners
        banners.update(scene);
        // 7. Update routes
        routes.update();
        // 8. Update morphs
        morphs.update();
    }
}

// *********************** //
// Rendering //
// *********************** //

void Layer::render_self(RenderBuffer &buffer) {
    // If we have no gears, we return
    if (gears.empty()) return;

    // If we're z-ordered, we sort based on that
    std::sort(gears.begin(), gears.end(), [](const Gear* e1, const Gear* e2) {
        return e1->z_index < e2->z_index;
    });

    // If the debug grid is enabled
    if (is_active() && core.debugger.is_grid())
        render_grid(buffer, 16, 1, true);

    // We render our world here

    if (world.get_texture_id() != -1)
        buffer.request_texture(world.get_texture_id());
    world.render_world(buffer);

    // We render our gears here.
    render_gears(buffer);

    // Holy we're done!
    // Wait what about debug mode
    if (is_active() && core.debugger.is_hitboxes())
        render_hitboxes(buffer);
}

void Layer::render_gears(RenderBuffer& buffer) {
    for (auto & gear : gears) {
        // Reference our gear for faster access.
        auto& g = *gear;
        // If our texture ID doesn't match, we end the batch and create a new one (if -1, it doesn't have a texture, so it's chill)
        buffer.request_texture(g.texture_id);
        // Adds the gear's vertices to the buffer.
        fstring<800> ginfo;
        str_view f = ginfo.wrap();
        g.to_fstring_verbose(f);
        g.to_vertex(buffer);
    }
}

void Layer::render_grid(RenderBuffer& buffer, int grid_div, int thickness, bool subdiv) const {
    Pos2D startpos = {static_cast<int>(camera.pos.x) - static_cast<int>(camera.pos.x)%grid_div, static_cast<int>(camera.pos.y) - static_cast<int>(camera.pos.y)%grid_div};
    Pos2D endpos = {};
    for (int i = startpos.x; i < camera.right(); i+= grid_div) {
        Box2D line = {i, static_cast<int>(camera.pos.y), thickness, camera.get_height()};
        gch::vector<SDL_FPoint> points;
        line.to_vert_points(points);
        for (auto& i: points)
            buffer.push_back(i, {255, 255, 255, 40});
    }
    for (int i = startpos.y; i < camera.bottom(); i+= grid_div) {
        Box2D line = {static_cast<int>(camera.pos.x), i, camera.get_width(), thickness};
        gch::vector<SDL_FPoint> points;
        line.to_vert_points(points);
        for (auto& i: points)
            buffer.push_back(i, {255, 255, 255, 40});
    }

    if (subdiv) {
        for (int i = startpos.x + grid_div/2; i < camera.right(); i+= grid_div) {
            Box2D line = {i, static_cast<int>(camera.pos.y), thickness, camera.get_height()};
            gch::vector<SDL_FPoint> points;
            line.to_vert_points(points);
            for (auto& i: points)
                buffer.push_back(i, {255, 255, 255, 15});
        }
        for (int i = startpos.y + grid_div/2; i < camera.bottom(); i+= grid_div) {
            Box2D line = {static_cast<int>(camera.pos.x), i, camera.get_width(), thickness};
            gch::vector<SDL_FPoint> points;
            line.to_vert_points(points);
            for (auto& i: points)
                buffer.push_back(i, {255, 255, 255, 15});
        }
    }
}

void Layer::render_hitboxes(RenderBuffer& buffer) {
    // we need to go through and render all the hitboxes
    for (auto & gear : gears) {
        auto& g = *gear;
        gch::vector<SDL_FPoint> hitbox = g.t.to_vertex_hitbox(1);
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
        gch::vector<SDL_FPoint> hitbox = c->to_vertex_hitbox(1);
        buffer.push_back(hitbox, {255, 0, 0, 255});
    }
}

void Layer::order() {
    // If the Layer is z-indexed
    if (scene.is_z_indexed()) {
        std::sort(gears.begin(), gears.end(), [](const Gear* e1, const Gear* e2) {
            return e1->z_index < e2->z_index;
        });
    }
}

SDL_Texture * Layer::_find_texture(int texture_id) {
    return texreg.get_texture(texture_id).texture;
}

bool Layer::is_active() const {
    return scene.is_active();
}

fstring<10> Layer::get_name() const {
    return name;
}

// ********************** //
// Private Functions //
// ********************** //

bool Layer::add_gear(Gear *g) {
    if (g->id == -1)
        g->id = id;
    else {
        glog::warn.src((get_name() << "::add_gear").cstr()) << "Cannot add item, as it has already been assigned to a layer";
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
    sprites.add_sprite(a);
}

void Layer::add_sprites(gch::vector<Sprite*>& new_sprites) {
    for (auto& a: new_sprites) {
        a->_engine_flagger(GFlag::sprite);
        add_gear(a);
    }
    sprites.add_sprites(new_sprites);
}

void Layer::remove_sprite(Sprite* a) {
    if (a == nullptr) return;
    particles.dissolve(a);
    morphs.strip_morph(a);
    sprites.dissolve(a);
    remove_gear(a);
}

void Layer::remove_sprites(gch::vector<Sprite*>& objs) {
    for (const auto& i: objs) {
        remove_sprite(i);
    }
}


// ********************** //
// Banners //
// ********************** //
void Layer::add_banner(Banner *b) {
    banners.add_banner(b);
    b->_engine_flagger(GFlag::banner);
    add_gear(b);
}

void Layer::add_banners(gch::vector<Banner *> &banners) {
    for (const auto& i: banners) {
        add_banner(i);
        add_gear(i);
    }
}

void Layer::remove_banner(Banner *b) {
    banners.remove_banner(b);
    remove_gear(b);
}

void Layer::remove_banners(gch::vector<Banner*>& banners) {
    for (const auto& i: banners) {
        remove_banner(i);
        remove_gear(i);
    }
}

Font & Layer::get_font(int index) {
    return fonts[index];
}

FrameTable & Layer::get_frame_table(int index) {
    return frames.get_table(index);
}


// ********************** //
// Particles //
// ********************** //
void Layer::instantiate_particle(ParticleGroup *pg) {
    if (add_gear(pg)) {
        // Define our element
        pg->_engine_flagger(GFlag::particle);
        // Add to our particle manager
        particles.add(pg);
        // Add it as a gear.
    }
    add_gear(pg);
}

void Layer::attach_particle(Gear *g, ParticleGroup *pg) {
    instantiate_particle(pg);
    pg->payload = g;
}

void Layer::attach_particles(gch::vector<ParticleGroup*>& pgs) {
    for (auto&i : pgs)
        instantiate_particle(i);
}

void Layer::detach_particle(Sprite *a) {
    particles.dissolve(a);
}

void Layer::detach_particle(ParticleGroup* pg) {
    if (pg == nullptr) return;
    if (pg->id != id)
        scene.log(1, "Item does not belong to layer.", "Layer::detach_particle()");
    else {
        particles.remove(pg);
        remove_gear(pg);
    }
}

void Layer::detach_particles(gch::vector<Sprite*>& pg) {
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
        morphs.apply_morph(e);
}

void Layer::strip_morph(Gear *g) {
    if (g->id != id)
        scene.log(1, "Item does not belong to layer.", "Layer::strip_morph");
    else
        morphs.strip_morph(g);
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
        morphs.strip_morph(e);
}

bool Layer::has_morph(Gear* g) {
    if (g->id != id)
        scene.log(1, "Item does not belong to layer.", "Layer::has_morph");
    else
        return morphs.has_morph(g);
    return false;
}

// *********************** //
// Routes //
// *********************** //
void Layer::set_route(Route *p) {
    if (p->get_payload()->id != id)
        scene.log(1, "Cannot add route, as payload does not belong to layer.", "Layer::set_route");
    routes.add_route(p);
}

void Layer::set_route(Route *r, Gear* g, const FPos2D &offset) {
    if (r->get_payload()->id != id)
        scene.log(1, "Cannot add route, as payload does not belong to layer.", "Layer::set_route");
    routes.add_route(r, g, offset);
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

void Layer::load_world(hstring filename, int tileset_num) {
    world.load_world(filename, &tiles[tileset_num]);
    physics.load_current_level();
}

Tileset & Layer::get_tileset(int index) {
    return tiles[index];
}

FPos2D Layer::get_scene_center() const {
    return {core.get_width()/2.f, core.get_height()/2.f};
    return {-1, -1};
}






