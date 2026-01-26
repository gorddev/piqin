#include "../../include/engine/core/Engine.hpp"

#include <SDL2/SDL_image.h>


#include "../../include/engine/debug/geng_debug.hpp"
#include "engine/debug/Console.hpp"

#define SDL_HINT_TRACKPAD_IS_TOUCH_ONLY "SDL_TRACKPAD_IS_TOUCH_ONLY"

using namespace gan;

Engine::Engine() : core(), rend(core),
                   layers(core),
                   console(nullptr), input(core) {
    glog::note.src("::~") << "Engine creation complete." <<
            glog::endlog;
}

Engine::~Engine() {
    for (auto& l : layers.get_layer_list())
        destroy_layer(l);
}

void Engine::init(bool debug_mode) {
    // Initialize SDL Video
    SDL_Init(SDL_INIT_VIDEO);
    glog::note.src("::~") << "SDL_Backend Formed" << glog::endlog;
    TTF_Init();
    glog::note.src("::~") << "SDL_TTF Initialized" << glog::endlog;
    rend._init();
    glog::note.src("::~") << "Window & Renderer Initialized" << glog::endlog;

    // Initialize our input
    input._init();

    // create our system font
    core.fonts.instantiate_font(Font(8, 16, 145, 103, 1, 0));

    if (debug_mode) {
        // If debug is enabled, create the router
        console = create_router<debug::Console>(core);
        glog::note << "Debug Mode Instantiated" << glog::endlog;
    }
    else
        console = nullptr;

    glog::note.src("root") << "Engine initialized." << glog::endlog;
}

bool Engine::tick(double time) {
    // First we update the engine_context with our time
    core.update(time);

    // Next we get input for the input manager and send to the relevant layer.
    bool quit = false;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        input.process_event(e, layers.get_active_layer());
        quit = (e.type == SDL_QUIT);
    }
    input.update(layers.get_active_layer());

    // WIP: Check if there's any layer change requests (this will be updated to a request interface later)
    gan::fstring<10> layerchange = core.get_layer_change();
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

    return !quit;
}


bool Engine::update() {
    // Next we update each of the active layers.
    for (auto& l : layers.get_layer_list()) {
        l->update(core.get_dt());
    }
    // Finally we update all of the input routers for post-processing
    input.post_update(get_active_layer());
    return true;
}

int Engine::create_font(hstring path, uint16_t spacing, uint16_t pt, SDL_ScaleMode render_mode) {
    auto font = core.fonts.create_font(path, spacing, pt);
    font->set_texture_id(rend.render_font(font, path, render_mode));
    return font->get_texture_id();
}

Font* Engine::get_font(int index) {
    return &core.fonts.at(index);
}

void Engine::render() {
    // We go ahead and render each of the active layers
    rend.render(layers.get_layer_list(), core.debugger.is_debug() ? console : nullptr);
    // Then we present our render
    rend.present();
}

void Engine::set_resolution(dim2 d) {

    if (console && d.w < 600) {
        while (d.w < 600) {
            d.w*=2;
            d.h*=2;
        }
        glog::warn.src("Engine::set_resolution") <<
            "Cannot set resolution below 600px in debug mode."
            "Resizing to: {" << d.w << ", " << d.h << "}"
            << glog::endlog;
        core._set_window_size(d.w, d.h);
        rend.set_render_resolution(d.w, d.h);
        console->notify_screen_resolution_change(d);
    }
    else {
        core._set_window_size(d.w, d.h);
        rend.set_render_resolution(d.w, d.h);
        if (console != nullptr)
            console->notify_screen_resolution_change(d);
    }

}

dim2 Engine::get_resolution() const {
    return {core.get_width(), core.get_height()};
}


void Engine::set_active_layer(Layer* layer) {
    set_active_layer(layer->scene.name);
}

void Engine::set_active_layer(gan::fstring<10> name) {
    layers.set_active_layer(name);
}

void Engine::increment_active_layer() {
    layers.increment_active_layer();
}

Layer * Engine::get_active_layer() {
    return layers.get_active_layer();
}

void Engine::destroy_layer(Layer *l) {
    destroy_layer(l->scene.name);
}

void Engine::destroy_layer(gan::fstring<10> layer_name) {
    layers.remove_layer(layer_name);
}

Layer *Engine::get_layer(gan::fstring<10> layer_name) {
    return layers.get_layer(layer_name);
}

void Engine::remove_router(InputRouter *router) {
    input.remove_input_router(router);
}

void Engine::set_debug_mode(bool enabled) {
    if (enabled && console != nullptr)
        core.enable_debug();
    else
        core.disable_debug();
}

void Engine::shutdown() {
    SDL_DestroyRenderer(rend.renderer);
    SDL_DestroyWindow(rend.window);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}
