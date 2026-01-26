#pragma once

#include "EngineContext.hpp"
#include "engine/input/InputDistributor.hpp"
#include "engine/rendering/Renderer.hpp"
#include "engine/layers/LayerManager.hpp"

namespace gan {
    /**
     * @brief The engine handles input, morph, particles, ECS, rendering, sound, and text. You do not have direct control over how the engine does its job. You only feed it inputs, and it gives you an output, and sends you keyboard input.
     * @details In order to use the engine, you have to speak in its language: @code ganine::Sprite@endcode, @code ganine::Morph@endcode, @code ganine::ParticleGroup@endcode, @code ganine::inputTarget@endcode, @code ganine::Font@endcode, and @code ganine::Sound@endcode. It is recommended to review these files before continuing with the engine.
     * Using the engine's features can be a bit of a hurdle to get used to, but it is designed to be somewhat intuitive.
     * - For @code Sprites@endcode: @code layer.add_sprite(...)@endcode and @code engine.remove_sprite(...)@endcode
     * - For @code Particles@endcode: @code layer.attach_particle(...)@endcode and @code engine.detach_particle(...)@endcode
     * - For @code Morph@endcode: @code layer.apply_morph(...)@endcode and @code engine.strip_morph(...)@endcode
     */
    class Engine {
    private:
        /// Keeps track of all the essential data for our engine to run.
        EngineContext core;
        /// Engine's Renderer. Very very off-limits no touching. Like seriously do not touch it.
        Renderer rend;
        /// Keeps track of all our layers so we can send them to the Renderer.
        LayerManager layers;
        /// The console of the layer
        debug::Console* console;
    public:
        /// Engine's InputDistributor. Call this to add routers to the engine.
        InputDistributor input;

        /// Constructor/destructor
        Engine();
        /// Default destructor—no pointers in engine.
        ~Engine();

        // *******************
        // <><> Engine Vitals <><>
        // *******************
        /// Initializes the engine. Must be called in @code int main(...)@endcode
        void init(bool debug_mode);
        /// Ticks the timer on the engine
        bool tick(double time);
        /// Runs the entire engine. Must call every frame, and put in the current time.
        bool update();

        // *******************
        // <><> Fonts <><>
        // *******************
        int create_font(hstring path, uint16_t spacing, uint16_t pt, SDL_ScaleMode render_mode = SDL_ScaleModeLinear);
        Font* get_font(int index);
        // *******************
        // <><> Rendering <><>
        // *******************
        /// Sends things to the renderer and presents it. Should call every frame.
        void render();
        /// Sets the resolution of the window
        void set_resolution(dim2 d);
        /// Gets the resolution of the window
        [[nodiscard]] dim2 get_resolution() const;


        // *******************
        // <><> Layers <><>
        // *******************
        /// Adds a layer to the engine
        /// Typename... Args
        template <typename T, typename... Args>
        T* create_layer(Args&&... args);
        // <><> Active Layer <><><>
        /// Sets the active layer that recieves input to the specified layer.
        void set_active_layer(Layer *layer);
        /// sets the active layer with the specified name.
        void set_active_layer(gan::fstring<10> name);
        /// Increases the active layer by one
        void increment_active_layer();
        /// Gets the active layer
        Layer* get_active_layer();

        // <><> Destroying Layers <><><>
        /// Removes a layer from the engine and destroys it based on layer pointer
        void destroy_layer(Layer *l);
        /// Removes a layer from the engine and destroys it based on layer name
        void destroy_layer(gan::fstring<10> layer_name);
        // <><> Getting Layer <><><>
        /// Gets a layer based on the layer name
        Layer *get_layer(gan::fstring<10> layer_name);

        // ***************************
        // <><> Input Management <><><>
        // ***************************
        template <typename T, typename... Args>
        T* create_router(Args&&... args);
        /// Removes an InputRouter from the InputDistributor
        void remove_router(InputRouter* router);

        // *********************
        // <><> Debugging <><><>
        // *********************
        /// Enables debugging modes in the engine
        void set_debug_mode(bool enabled);

        /// Shutodnw
        void shutdown();

    };
}

#include "engine/core/Engine.inl"
