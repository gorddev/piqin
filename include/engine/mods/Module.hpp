#pragma once

#include "../layers/layer-subobjects/LayerCore.hpp"
#include "engine/core/gears/Gear.hpp"
#include "engine/layers/layer-subobjects/GearList.hpp"

namespace gan {
    /// ModuleInitializer that provides a module
    class ModuleInit final {
        // Only ModuleManager can make this object
        friend class ModuleManager;
        // Explicit constructor
        explicit ModuleInit(LayerCore& layer_core)
            : scene(layer_core) {}
    public:
        LayerCore& scene;
        uint_fast16_t module_id;
    };

    /// @brief A Module is a subcomponent to a layer that can be installed into a layer to perform a specific function.
    /// For example, some pre-provided Modules include @code Animation@endcode, @code Banner@endcode, or @code Particles@endcode.
    /// Modules allow user-defined systems to be automatically implemented into the logic of the engine.
    ///
    struct Module {
        /// Cannot create a Module object without a subclass first, as ModuleInits can only be formed by the layer
        explicit Module(const ModuleInit& init)
            : scene(init.scene), module_id(init.module_id), _update_(0) {}

        /// Virtual destructor
        virtual ~Module() = default;

        // <><><> Update functions <><><>
        /// Virtual pre_update function. Called before any regular update() functions are called.
        virtual void pre_update() { _update_ &= NO_PRE_UPDATE; }
        /// Virtual update function. Called before rendering in order of when module was added
        virtual void update() { _update_ &= NO_UPDATE;}
        /// Virtual post_update function. Called after the first update pass
        virtual void post_update() { _update_ &= NO_POST_UPDATE; }
        /// Called when a gear has been removed so it can handle the logic internally
        virtual void on_gear_removal(Gear*) {}

        // <><><> Rendering functions <><><>
        /// If the module renders an overlay, specify the render logic here


    public:
        /// Contains the list of gears a module can append to
        LayerCore& scene;
        /// Enumerator bitwise object for modules
        enum ModuleUpdate : uint_fast8_t {
            NO_PRE_UPDATE = 0 << 1,
            NO_UPDATE = 1 << 1,
            NO_POST_UPDATE = 1 << 2
        };
        /// Contains the internal id of the module
        const uint_fast16_t module_id;
        /// Determines whether a module will be updated or not
        uint_fast8_t _update_;
    };
}
