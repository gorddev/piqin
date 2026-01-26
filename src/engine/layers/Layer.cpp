#include "engine/layers/Layer.hpp"

#include <utility>

using namespace gan;

Layer::Layer(LayerInit& layer_init, fstring<10> name)
    : core(layer_init.core), name(name),
      scene(this->name, camera, core.width, core.height, (ModuleManager::gear_removal_wrapper), &mods, core.tex_reg),
      mods(scene),
      input(scene) {
}

Layer::~Layer() {
    delete background;
}

void Layer::update(double dt) {
    if (scene.is_running()) {
        // 1. Grab time from the main loop
        scene._update(dt);

        // <><><> Modules <><><>
        // 2. Pre_Update phase
        mods.pre_update();
        // 2. Update phase
        mods.update();
        // 3. Post Update phase
        mods.post_update();
    }
}

// *********************** //
// Rendering //
// *********************** //

void Layer::render_self(RenderBuffer &buffer) {
    // If the debug grid is enabled
    if (is_active() && core.debugger.is_grid()) {
        buffer.begin_object();
        render_grid(buffer, 16, 1, true);
        buffer.end_object();
    }

    // We render our gear here
    scene.gears.render_gears(buffer);

    // Wait what about debug mode
    if (is_active() && core.debugger.is_hitboxes())
        scene.gears.render_hitboxes(buffer, scene.camera.get_width());
}

void Layer::render_grid(RenderBuffer& buffer, int grid_div, int thickness, bool subdiv) const {
    pos2 startpos = {static_cast<int>(camera.pos.x) - static_cast<int>(camera.pos.x)%grid_div, static_cast<int>(camera.pos.y) - static_cast<int>(camera.pos.y)%grid_div};
    pos2 endpos = {};
    for (int i = startpos.x; i < camera.right(); i+= grid_div) {
        box2 line = {i, static_cast<int>(camera.pos.y), thickness, camera.get_height()};
        std::vector<SDL_FPoint> points;
        line.to_tex_points(points);
        for (auto& i: points)
            buffer.push_back(i, {gan::max_alpha, gan::max_alpha, gan::max_alpha, 120});
    }
    for (int i = startpos.y; i < camera.bottom(); i+= grid_div) {
        box2 line = {static_cast<int>(camera.pos.x), i, camera.get_width(), thickness};
        std::vector<SDL_FPoint> points;
        line.to_tex_points(points);
        for (auto& i: points)
            buffer.push_back(i, {gan::max_alpha, gan::max_alpha, gan::max_alpha, 120});
    }

    if (subdiv) {
        for (int i = startpos.x + grid_div/2; i < camera.right(); i+= grid_div) {
            box2 line = {i, static_cast<int>(camera.pos.y), thickness, camera.get_height()};
            std::vector<SDL_FPoint> points;
            line.to_tex_points(points);
            for (auto& i: points)
                buffer.push_back(i, {gan::max_alpha, gan::max_alpha, gan::max_alpha, 65});
        }
        for (int i = startpos.y + grid_div/2; i < camera.bottom(); i+= grid_div) {
            box2 line = {static_cast<int>(camera.pos.x), i, camera.get_width(), thickness};
            std::vector<SDL_FPoint> points;
            line.to_tex_points(points);
            for (auto& i: points)
                buffer.push_back(i, {gan::max_alpha, gan::max_alpha, gan::max_alpha, 65});
        }
    }
}


void Layer::set_speed(float speed) {
    scene.state.set_speed(speed);
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
    return scene.gears.add_gear(g, 0);
}

void Layer::remove_gear(Gear* g) {
    scene.gears.remove_gear(g);
}

vec2 Layer::get_scene_center() const {
    return {core.get_width()/2.f, core.get_height()/2.f};
}





