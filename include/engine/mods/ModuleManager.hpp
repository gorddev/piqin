#pragma once
#include <typeindex>
#include <unordered_map>

#include "Module.hpp"
#include "ModuleBundle.hpp"
#include "engine/core/gears/Gear.hpp"

namespace gan {

    // *******************
    // Module IDing for no RTTI
    // *******************
    using ModuleTypeID = uint32_t;
    inline ModuleTypeID next_module_type_id() {
        static ModuleTypeID id = 0;
        return id++;
    }

    template<typename T>
    ModuleTypeID module_type_id() {
        static ModuleTypeID id = next_module_type_id();
        return id;
    }

    /// Contains all the user and system-installed modules for a layer
    class ModuleManager final {
    private:
        friend class Layer;
        /// Map from type to the specific module pointer
        std::unordered_map<ModuleTypeID, Module*> modules;
        /// Vector containing the ordering for the ModuleManager
        std::vector<Module*> order;
        /// Contains the LayerContext
        LayerCore& context;

    public:
        /// Wrapper function for notify_gear_removal
        static void gear_removal_wrapper(void* mod_manager, Gear* g, uint_fast16_t module_id) {
            static_cast<ModuleManager*>(mod_manager)->notify_gear_removal(g, module_id);
        }
        /// Notifies all modules when a gear has been removed
        void notify_gear_removal(Gear *g, uint_fast16_t module_id);
    private:
        /// Default constructor for the ModuleManager
        explicit ModuleManager(LayerCore& context);
    public:

        /// Adds a module to the ModuleManager with the specified type
        template<typename T, typename... Args>
            requires(std::derived_from<T, Module>) // requires that it is a type of module
        T* add_module(Args &&... args) {
            if (modules.contains(module_type_id<T>())) {
                glog::warn.src("ModuleManager::add_module<T>()") << "Unable to add module: type already exists." << glog::endlog;
                return get_module<T>();
            }
            // Create an initializer with the current gear list
            ModuleInit init(context);
            // Creates a new module from the arguments
            T* new_module = new T(init, std::forward<Args>(args)...);
            // Assign this new module into the unordered map
            modules[module_type_id<T>()] = new_module;
            // Add this new module to the vector of module
            order.push_back(new_module);
            // return our new module
            return new_module;
        }

        /// Adds multiple modules to the layer simultaneously as long as they take in no additional
        /// constructor arguments apart from a ModuleInit.
        template<typename... Ts>
        requires (std::derived_from<Ts, Module> && ...)
        void add_modules() {
            (add_module<Ts>(), ...);
        }

        /// Uses a module bundle to create multiple modules.
        template<typename... Mods>
        void add_bundle(ModuleBundle<Mods...>) {
            (add_module<Mods>(), ...);
        }

        /// Gets the specified module with the specified type
        template<typename T>
            requires(std::derived_from<T, Module>)
        T* get_module() {
            if (modules.contains(module_type_id<T>()))
                return static_cast<T*>(modules[module_type_id<T>()]);
            glog::err.src("ModuleManager::mod()") << "Module does not exist in layer."
                << glog::endlog;
            // here to avoid return error
            return static_cast<T*>(modules[module_type_id<T>()]);
        }

        /// Calls all module pre_update() functions
        void pre_update();

        /// Calls all module update() functions
        void update();

        /// Calls all module post_update() functions
        void post_update();

        /// Destructor, called on all Modules
        ~ModuleManager();
    };
}
