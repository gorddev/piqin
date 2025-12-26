#include "../../include/engine/core/Engine.hpp"


#include "../../include/engine/debug/geng_debug.hpp"
#include "engine/debug/DebugManager.hpp"

using namespace geng;

Engine::Engine() : core(camera), texreg(core), input(core), layers(core), rend(core, texreg, camera) {
    glog::note.src("root") << "Engine creation complete." << glog::endlog;
}

void Engine::init() {
    // Initialize SDL Video
    SDL_Init(SDL_INIT_VIDEO);
    rend._init();
    glog::note.src("root") << "Engine SDL Backend Formed" << glog::endlog;

    // Initialize our input
    input._init();

    // Then we create our debug router
    console = create_router<debug::DebugManager>(core);

    glog::note.src("root") << "Engine initialized." << glog::endlog;
}

void Engine::tick(double time) {
    // First we update the engine_context with our time
    core.update(time);

    // Next we get input for the input manager and send to the relevant layer.
    SDL_Event e;
    while (SDL_PollEvent(&e))
        input.process_event(e, layers.get_active_layer());
    input.update(layers.get_active_layer());

    // Check if there's any layer change requests (this will be updated to a request interface later)
    geng::fstring<10> layerchange = core.get_layer_change();
    if (layerchange.length()) {
        if (layerchange == "__next")
            increment_active_layer();
        else if (layerchange == "__run")
            get_active_layer()->scene.toggle_running();
        else if (layerchange == "__visible") {
            get_active_layer()->scene.toggle_visible();
        }
        else
            set_active_layer(layerchange);
        core.set_layer_change("");
    }
}


bool Engine::update() {
    // Next we update each of the active layers.
    for (auto& l : layers.get_layer_list()) {
        l->update(core.get_dt());
    }
    return true;
}

void Engine::render() {
    // We go ahead and render each of the active layers
    rend.render(layers.get_layer_list(), core.debugger.is_debug() ? console : nullptr);
    // Then we present our render
    rend.present();
}

void Engine::set_resolution(Dim2D d) {
    core._set_window_size(d.w, d.h);
    rend.set_render_resolution(d.w, d.h);
    console->notify_screen_resolution_change(d);
}

Dim2D Engine::get_resolution() const {
    return {core.get_width(), core.get_height()};
}


void Engine::set_active_layer(Layer* layer) {
    set_active_layer(layer->scene.get_name());
}

void Engine::set_active_layer(geng::fstring<10> name) {
    layers.set_active_layer(name);
}

void Engine::increment_active_layer() {
    layers.increment_active_layer();
}

Layer * Engine::get_active_layer() {
    return layers.get_active_layer();
}

void Engine::destroy_layer(Layer *l) {
    destroy_layer(l->scene.get_name());
}

void Engine::destroy_layer(geng::fstring<10> layer_name) {
    layers.remove_layer(layer_name);
}

Layer *Engine::get_layer(geng::fstring<10> layer_name) {
    return layers.get_layer(layer_name);
}

void Engine::remove_router(InputRouter *router) {
    input.remove_input_router(router);
}

void Engine::set_debug_mode(bool enabled) {
    if (enabled)
        core.enable_debug();
    else
        core.disable_debug();
}
