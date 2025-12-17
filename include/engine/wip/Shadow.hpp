#pragma once
#include <SDL_render.h>
#include <utility>
#include <functional>
#include <iostream>
#include <vector>


namespace geng {

    /// Renders shadows directly onto the background
    void shadow_background(std::vector<SDL_Vertex>& buffer, int& numVertices);
    /// Renders shadows onto a given floor.
    void shadow_floor(std::vector<SDL_Vertex>& buffer, int& numVertices);

    using shadowFunc = std::function<void(std::vector<SDL_Vertex>& buffer, int& numVertices)>;

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
    class ShadowCalc final  {
    private:
        shadowFunc currentFunc = nullptr;
        int floor = 50;
    public:

        ShadowCalc() {
            currentFunc = shadowfuncs["background"];
        }
        std::unordered_map<std::string, shadowFunc> shadowfuncs {
            {"background", shadow_background},
            {"floor", shadow_floor}
        };

        void set_function(const std::string &shadow) {
            if (shadowfuncs.find(shadow) != shadowfuncs.end()) {
                currentFunc = shadowfuncs[shadow];
            }
            else {
                std::cerr << "FATAL: Shadow function does not exist.\n";
                abort();
            }
        }
        void add_function(const std::string& shadow, shadowFunc func) {
            if (shadowfuncs.find(shadow) != shadowfuncs.end()) {
                std::cerr << "Err adding shadow: function already exists.\n";
            }
            else {
                shadowfuncs[shadow] = std::move(func);
            }
        }
        void apply_shadow(std::vector<SDL_Vertex>& buffer, int numVertices) {
            if (buffer.size() < numVertices) {
                std::cerr << "ERR: Requesting more shadows than Vertices rendered.\n";
                numVertices = buffer.size();
            }
            currentFunc(buffer, numVertices);
        }
        void set_floor(int shadowFloor) {
            floor = shadowFloor;
        }
    };

    /// Returns our shadow
    ShadowCalc& get_shadow_calc();
}
