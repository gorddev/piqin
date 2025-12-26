#pragma once

#include "Route.hpp"
#include "engine/layers/LayerContext.hpp"

namespace geng {
    /** @brief Updates all paths such that they affect their assigned @code gengine::Transform2D@endcode object correctly.
     * @details Constains a @code std::unordered_map<Path*>@endcode and defines how it can be interacted with.
     * The following member functions apply:
     * 1. @code PathManager()@endcode › default constructor
     * 2. @code void add_path(Transform2D& t, Path* p)@endcode › adds a path to the given transform
     * 3. @code void remove_path(Transform2D& t, Path* p)@endcode › removes a path from the given transform
     * 4. @code void update()@endcode › updates all paths in the PathManager, and deletes finished paths.
     * 5. @code ~PathManager()@endcode › deletes all paths in the PathManager
     */
    class RouteManager {
        /// A SparseVector containing all the paths. Destroyed paths are nullptrs.
        std::unordered_map<uint32_t, Route*> paths;
        /// Holds the LayerContext
        LayerContext& scene;
    public:
        /// Default constructor for RouteManager
        explicit RouteManager(LayerContext& scene)
            : scene(scene) {}

        /// Adds a path to the path manager
        void add_route(Route *p);
        /// Adds a path, targeted at the provided Gear with a given offset.
        void add_route(Route *p, Gear* g, const FPos2D &offset = {0.f,0.f});

        /// Removes a path from the given transform
        void remove_path(const Gear* g);
        /// Removes a path with the given path pointer (not recommended to use because not O(1))
        void remove_path(const Route *path);

        /// Updates all paths in the RouteManager
        void update();

        /// Destructor that destroys all paths in the RouteManager
        ~RouteManager();

    };
}
