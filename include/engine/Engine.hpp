#pragma once

#include "EngineContext.hpp"
#include "debug/Console.hpp"
#include "input/InputDistributor.hpp"
#include "rendering/Renderer.hpp"
#include "engine/layers/LayerManager.hpp"

namespace geng {
    /**
     * @brief The engine handles input, morph, particles, ECS, rendering, sound, and text. You do not have direct control over how the engine does its job. You only feed it inputs, and it gives you an output, and sends you keyboard input.
     * @details In order to use the engine, you have to speak in its language: @code gengine::Sprite@endcode, @code gengine::Morph@endcode, @code gengine::ParticleGroup@endcode, @code gengine::inputTarget@endcode, @code gengine::Font@endcode, and @code gengine::Sound@endcode. It is recommended to review these files before continuing with the engine.
     * Using the engine's features can be a bit of a hurdle to get used to, but it is designed to be somewhat intuitive.
     * - For @code Sprites@endcode: @code layer.add_sprite(...)@endcode and @code engine.remove_sprite(...)@endcode
     * - For @code Particles@endcode: @code layer.attach_particle(...)@endcode and @code engine.detach_particle(...)@endcode
     * - For @code Morph@endcode: @code layer.apply_morph(...)@endcode and @code engine.strip_morph(...)@endcode
     */
    class Engine {
    public:
        /// Camera of the layer
        Camera camera;
    private:
        /// Keeps track of all the essential data for our engine to run.
        EngineContext core;
        /// Keeps track of all of the currently loaded textures
        TextureRegister texreg;
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
        ~Engine() = default;

        // *******************
        // <><> Engine Vitals <><>
        // *******************
        /// Initializes the engine. Must be called in @code int main(...)@endcode
        void init();
        /// Runs the entire engine. Must call every frame, and put in the current time.
        bool tick(double time);

        // *******************
        // <><> Rendering <><>
        // *******************
        /// Sends things to the renderer and presents it. Should call every frame.
        void render();
        /// Sets the resolution of the window
        void set_resolution(Dim2D d);
        /// Gets the resolution of the window
        [[nodiscard]] Dim2D get_resolution() const;


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
        void set_active_layer(geng::fstring<10> name);
        /// Increases the active layer by one
        void increment_active_layer();
        /// Gets the active layer
        Layer* get_active_layer();

        // <><> Destroying Layers <><><>
        /// Removes a layer from the engine and destroys it based on layer pointer
        void destroy_layer(Layer *l);
        /// Removes a layer from the engine and destroys it based on layer name
        void destroy_layer(geng::fstring<10> layer_name);
        // <><> Getting Layer <><><>
        /// Gets a layer based on the layer name
        Layer *get_layer(geng::fstring<10> layer_name);


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

    };
}

#include "engine/Engine.inl"
