#pragma once
#include "layer-subobjects/LayerCore.hpp"
#include "LayerInit.hpp"
#include "engine/input/InputHandler.hpp"
#include "engine/mods/ModuleManager.hpp"
#include "engine/scene/initializer/Initializer.hpp"
#include "engine/rendering/background/Background.hpp"
#include "engine/scene/cells/CellBucket.hpp"
#include "engine/mods/events/EventModule.hpp"

namespace gan {

    /** @brief The Layer is the primary target for adding objects, particles, ect. Each Layer operates independently of other layers.
     * @details Each layer by defualt is not z-indexed and is visible. To change this, access the relevant member
     * functions such as @code set_strict()@endcode or @code set_z_indexed()@endcode. Every layer has the following member funcctions
     */
    class Layer {
    private:
        /// Also keeps track of the core of the engine, like camera and scene width/height.
        EngineContext& core;
        /// Name of the layer.
        const fstring<10> name;
    public:
        /// Keeps track of the camera
        Camera camera;
        /// The scene contains important information concerning the layer.
        /// Most of the member variables below receive a reference to this scene object.
        LayerCore scene;
        /// Holds all the modules within our layer
        ModuleManager mods;
        /// Allows the user to access the input of the layer.
        InputHandler input;
        /// Allows users to directly add cells.[NOT RECOMMENDED FOR GAMEPLAY PURPOSES]
        CellBucket cell_bucket;
    private:
        // <><><> Texture Manipulation/Retrieval
        /// The background of the Layer
        Background* background = nullptr;

        /// Used for id-assignmnet
        int id = 0;
        /// Adds one element to the Layer
        bool add_gear(Gear* g);
        /// Removes the specifies element from the Layer
        void remove_gear(Gear* g);
    public:
        /// Constructor for a Layer
        explicit Layer(LayerInit& layer_init, gan::fstring<10> name);
        /// Destructor for a Layer
        virtual ~Layer();

        /// Updates all elements in a scene. Override if you know what you're doing.
        virtual void update(double dt);

        // <><><> Mods <><><>
        /// Returns a mod from the module manager
        template<typename T>
            requires(std::derived_from<T, Module>)
        T* mod();

        // <><><> Returns the center of the scene <><><>
        [[nodiscard]] vec2 get_scene_center() const;

        // <><><> Returns whether we're active or not.
        [[nodiscard]] bool is_active() const;
        // <><><> Returns the name of the layer
        [[nodiscard]] fstring<10> get_name() const;
        /// Sets the speed of the layer
        void set_speed(float speed);
    protected:
        friend class Engine;
        friend class Renderer;
        // <><><> Engine-Specific Functions <><><>
        /// Called by the engine to render itself. Only override if you really trust yourself.
        virtual void render_self(RenderBuffer &buffer);
    private:
        /// Renders the debug grid in the layer
        void render_grid(RenderBuffer &buffer, int grid_div, int thickness, bool subdiv = false) const;

    };
}

// Module grabber definition
template<typename T>
    requires (std::derived_from<T, gan::Module>)
T* gan::Layer::mod() {
    return mods.get_module<T>();
}
