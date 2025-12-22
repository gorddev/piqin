#include "engine/Engine.hpp"

#include "engine/debug/DebugRouter.hpp"

using namespace geng;

Engine::Engine() : rend(context), input(context), layers(context) {}

void Engine::init() {
    // Initialize SDL Video
    SDL_Init(SDL_INIT_VIDEO);
    rend._init();
    input._init();

    // Then we create our debug router
    input.add_input_router(new debug::DebugRouter(context.debugger, context));
}


bool Engine::tick(double time) {
    // First we update the engine_context with our time
    context.update(time);

    std::string layerchange = context.get_layer_change();
    if (layerchange != "") {
        if (layerchange == "__next")
            increment_active_layer();
        else
            set_active_layer(layerchange);
        context.set_layer_change("");
    }

    // Next we get input for the input manager and send to the relevant layer.
    SDL_Event e;
    while (SDL_PollEvent(&e))
        input.process_event(e, layers.get_active_layer());
    input.update(layers.get_active_layer());
    // Next we update each of the active layers.
    for (auto& l : layers.get_layer_list())
        l->update(context.get_dt());

    // Will add boolean functionality later
    return true;
}

void Engine::render() {
    // We go ahead and render each of the active layers
    rend.render(layers.get_layer_list());
    // Then we present our render
    rend.present();
}

void Engine::set_resolution(Dim2D d) {
    rend.set_render_resolution(d.w, d.h);
}

Dim2D Engine::get_resolution() const {
    return {context.get_width(), context.get_height()};
}

void Engine::compose_layer(Layer *l) {
    direct_log(0, "Composing layer: " +l->scene.get_name(), "compose_layer");
    rend.prime_tex_register(l->_init());
    direct_log(0, "Success: Composed layer: " +l->scene.get_name(), "compose_layer");
    layers.add_layer(l);
}

void Engine::set_active_layer(Layer* layer) {
    set_active_layer(layer->scene.get_name());
}

void Engine::set_active_layer(std::string name) {
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

void Engine::destroy_layer(std::string layer_name) {
    layers.remove_layer(layer_name);
}

Layer *Engine::get_layer(std::string layer_name) {
    return layers.get_layer(layer_name);
}

void Engine::add_input_router(InputRouter *router) {
    input.add_input_router(router);
}

void Engine::remove_input_router(InputRouter *router) {
    input.remove_input_router(router);
}

void Engine::direct_log(int severity, std::string msg, std::string src) {
    context.log(severity, msg, src);
}

void Engine::set_debug_mode(bool enabled) {
    if (enabled)
        context.enable_debug();
    else
        context.disable_debug();
}

void Engine::set_debug_immediate_print(bool enabled) {
    if (enabled)
        context.debugger.enable_immediate_print();
    else
        context.debugger.disable_immediate_print();
}
