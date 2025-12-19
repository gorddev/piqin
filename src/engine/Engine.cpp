#include "engine/Engine.hpp"

using namespace geng;

Engine::Engine() : rend(world, &cam), input(world), layer(world) {}

void Engine::init() {
    rend._init();
}

bool Engine::tick(double time) {
    // First we update the world with our time
    world.update(time);

    // Next we get input for the input manager and send to the relevant layer.
    SDL_Event e;
    while (SDL_PollEvent(&e))
        input.update(e, layer.get_active_layer());

    // Next we update each of the active layers.
    for (auto& l : layer.get_layer_list())
        l->update(world.get_dt());

    // Will add boolean functionality later
    return true;
}

void Engine::render() {
    // We go ahead and render each of the active layers
    rend.render(layer.get_layer_list());
    // Then we present our render
    rend.present();
}

void Engine::set_resolution(Dim2D d) {
    rend.set_render_resolution(d.w, d.h);
}

Dim2D Engine::get_resolution() const {
    return {world.get_width(), world.get_height()};
}

void Engine::compose_layer(Layer *l) {
    direct_log(0, "Composing layer: " +l->scene.get_name(), "compose_layer");
    rend.prime_tex_register(l->_init());
    direct_log(0, "Success: Composed layer: " +l->scene.get_name(), "compose_layer");
    layer.add_layer(l);
}

void Engine::set_active_layer(Layer* l) {
    layer.set_active_layer(l);
}

Layer * Engine::get_active_layer() {
    return layer.get_active_layer();
}

void Engine::destroy_layer(Layer *l) {
    layer.remove_layer(l);
    delete l;
}

void Engine::destroy_layer(std::string layer_name) {
    layer.remove_layer(layer_name);
}

void Engine::get_layer(std::string layer_name) {
    layer.get_layer(layer_name);
}

void Engine::direct_log(int severity, std::string msg, std::string src) {
    world.log(severity, msg, src);
}

void Engine::set_debug_mode(bool enabled) {
    if (enabled)
        world.enable_debug();
    else
        world.disable_debug();
}

void Engine::set_debug_immediate_print(bool enabled) {
    if (enabled)
        world.logs.enable_immediate_print();
    else
        world.logs.disable_immediate_print();
}
