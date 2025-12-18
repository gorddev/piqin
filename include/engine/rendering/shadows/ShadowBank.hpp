#pragma once
#include <SDL_render.h>
#include <functional>
#include <vector>

#include "shadow-types/ShadowBackground.hpp"
#include "shadow-types/ShadowFloor.hpp"


namespace geng {
    /// Alias so we don't have to write that out each time.
    using shadowFunc = std::function<void(std::vector<SDL_Vertex>& buffer, int& numVertices, void* userdata)>;

    /** @brief The shadow calculator takes in a buffer of vertices, and renders a shadow behind the number of provided vertices.
     * @details The shadow calculator can dynamically change it's shadow render method on the fly with the set_function(...) method. Two shadow casting methods are pre-provided
     * which can be selected with @code set_function("background")@endcode and @code set_function("floor")@endcode. However, you are free to program shadow calculation functions yourself with
     * the @code add_function(std::string, std::function<void(SDL_Vertex>&buffer, int& numVertices)>)@endcode method. ShadowCalc is intended to be accessed exclusively through the @code shadow_calc@endcode inline global variable.
     * ShadowCalc has the following member variables and member functions
     * - @code shadowFunc currentFunc@endcode › current shadow rendering function
     * - @code int floor@endcode › determines the floor by which the floor calculation method calculates shadows
     * - @code unordered_map<string, shadowFunc>@endcode › Links shadow names to their respective functions
     * - @code set_function(std::string)@endcode › Sets the current shadow in accordance with the string
     * - @code add_function(std::string, shadowFunc)@endcode › Adds a shadow rendering method to the map of shadow functions
     * - @code apply_shadow(...)@endcode › Used exclusively by Actors, ParticleGroups, and Panels to render themselves
     * - @code set_floor(...)@endcode › Sets the floor for the floor rendering method
     */
    class ShadowBank final  {
    private:
        shadowFunc currentFunc = nullptr;
        int floor = 50;
    public:
        /// Unordered map containing all banked shaow functions.
        std::unordered_map<std::string, shadowFunc> shadowfuncs {
                {"background", gfx::shadow_background},
                {"floor", gfx::shadow_floor}
        };

        /// Constructor for the shadowbank.
        ShadowBank();

        /// Sets the current function via the provided string
        void set_function(const std::string &shadow);
        /// Adds a shadow function to the current shadowBank
        void add_function(const std::string& shadow, shadowFunc func);
        /// Applies a shadow to a buffer
        void apply_shadow(std::vector<SDL_Vertex>& buffer, int numVertices, std::string shadow_type);
        /// Applies a shadow to a buffer
        void apply_shadow(std::vector<SDL_Vertex>& buffer, int numVertices = 3);
        /// Sets the floor for floor-based shadows
        void set_floor(int shadowFloor);
    };

}
