#pragma once

#include <iostream>

#include "EngineContext.hpp"
#include "input/InputSource.hpp"
#include "wip/Random.hpp"
#include "rendering/Renderer.hpp"
#include "engine/layers/LayerStack.hpp"
#include "types/positioning/Point2D.hpp"
#include "utilities/IDStack.hpp"


namespace geng {
    /**
     * @brief The engine handles input, morph, particles, ECS, rendering, sound, and text. You do not have direct control over how the engine does its job. You only feed it inputs, and it gives you an output, and sends you keyboard input.
     * @details In order to use the engine, you have to speak in its language: @code gengine::Actor@endcode, @code gengine::Morph@endcode, @code gengine::ParticleGroup@endcode, @code gengine::inputTarget@endcode, @code gengine::Font@endcode, and @code gengine::Sound@endcode. It is recommended to review these files before continuing with the engine.
     * Using the engine's features can be a bit of a hurdle to get used to, but it is designed to be somewhat intuitive.
     * - For @code Actors@endcode: @code layer.add_actor(...)@endcode and @code engine.remove_actor(...)@endcode
     * - For @code Particles@endcode: @code layer.attach_particle(...)@endcode and @code engine.detach_particle(...)@endcode
     * - For @code Morph@endcode: @code layer.apply_morph(...)@endcode and @code engine.strip_morph(...)@endcode
     */
    class Engine {
    private:
        /// Keeps track of all the essential data for our engine to run.
        EngineContext world;

        /// Engine's Renderer. Very very off-limits no touching. Like seriously do not touch it.
        Renderer rend;
        /// You can touch the camera tho it's fine.
        Camera cam;
        /// Keeps track of all our layers so we can send them to the Renderer.
        LayerStack layer;
    public:

        /// Engine's InputSource.
        InputSource input;

        // Constructor/destructor
        Engine();
        ~Engine() = default;

        // *******************
        // <><> Engine Vitals <><>
        // *******************
        /// Initializes the engine. Must be called in @code int main(...)@endcode
        void init();

        /// Runs the entire engine. Must call every frame, and put in the current time.
        bool tick(double time);

        /// Sends things to the renderer and presents it. Should call every frame.
        void render();

        // *******************
        // <><> Rendering <><>
        // *******************
        /// Sets the resolution of the window
        void set_resolution(Dim2D d);
        /// Gets the resolution of the window
        [[nodiscard]] Dim2D get_resolution() const;

        // *******************
        // <><> Layers <><>
        // *******************
        /// Adds a layer to the engine and composes uninitialized elements. Can be called multiple times on the same layer in case you add additional textures or frame tables.
        void compose_layer(Layer *l);
        // <><> Active Layer <><><>
        /// Sets the active layer that recieves input to the specified layer.
        void set_active_layer(Layer *l);
        /// Gets the active layer
        Layer* get_active_layer();
        // <><> Destroying Layers <><><>
        /// Removes a layer from the engine and destroys it based on layer pointer
        void destroy_layer(Layer *l);
        /// Removes a layer from the engine and destroys it based on layer name
        void destroy_layer(std::string layer_name);
        // <><> Getting Layer <><><>
        /// Gets a layer based on the layer name
        void get_layer(std::string layer_name);

        // *********************
        // <><> Debugging <><><>
        // *********************
        /// Directly logs from engine root. Usually not recommended to do.
        void direct_log(int severity, std::string msg, std::string src = "");
        /// Enables debugging modes in the engine
        void set_debug_mode(bool enabled);
        /// Immediately prints all debug notifications
        void set_debug_immediate_print(bool enabled);
    };
}
