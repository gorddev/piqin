#pragma once

#include "Route.hpp"
#include "../../layers/layer-subobjects/LayerCore.hpp"
#include "engine/mods/Module.hpp"

namespace gan {
    /** @brief Updates all paths such that they affect their assigned @code gan::Transform2D@endcode object correctly.
     * @details Contains a @code std::unordered_map<Path*>@endcode and defines how it can be interacted with.
     * The following member functions apply:
     * 1. @code PathManager()@endcode › default constructor
     * 2. @code void add_path(Transform2D& t, Path* p)@endcode › adds a path to the given transform
     * 3. @code void remove_path(Transform2D& t, Path* p)@endcode › removes a path from the given transform
     * 4. @code void update()@endcode › updates all paths in the PathManager, and deletes finished paths.
     * 5. @code ~PathManager()@endcode › deletes all paths in the PathManager
     */
    class RouteModule : public Module {
        /// A SparseVector containing all the paths. Destroyed paths are nullptrs.
        std::vector<Route*> paths;
        /// A map from gears to routes
        std::unordered_map<Gear*, Route*> gear_to_routes;
    public:
        /// Default constructor for RouteManager
        explicit RouteModule(ModuleInit init)
            : Module(init) {}

        /// Adds a path to the path manager
        void add_route(Route *p);
        /// Adds a path, targeted at the provided Gear with a given offset.
        void add_route(Route *p, Gear* g, const vec2 &offset = {0.f,0.f});

        /// Removes a path from the given transform
        void remove_path(Gear* g);
        /// Removes a path with the given path pointer (not recommended to use because not O(1))
        void remove_path(const Route *path);

        /// Returns true if the current gear has a route
        bool has_route(Gear* g);

        /// Updates all paths in the RouteManager
        void update() override;

        /// Destructor that destroys all paths in the RouteManager
        ~RouteModule() override;
    };

    using Routes = RouteModule;
}
