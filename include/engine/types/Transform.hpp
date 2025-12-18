#pragma once
#include "positioning/Vertex.hpp"
#include "engine/EngineContext.hpp"
#include <SDL.h>


namespace geng {
    /** @brief The fundamental target for all morphs, routes, shaking, and rendering.
     * @details The Transform contains all the position information necessary for a game object to be rendered. Although @code geng::Gear@endcode s are the primary
     * holders of transforms, other object types, like user-defined subclasses of @code geng::Mesh@endcode can also hold transforms. We briefly cover
     * how the engine interfaces with @code geng::Transform@endcode.
     * - @code engine.render()@endcode Relies on the width, height, position, and offset within a transform to render @code geng::Actors@endcode
     * - @code geng::Vertex pos@endcode and @code geng::Vertex offset@endcode perform two different functions. @code pos@endcode keeps track of the absolute position
     * - Member variables below:
     * - @code uint16_t baseWidth, baseHeight@endcode › Base width and height of the object: private.
     * - @code float scale@endcode › scale of the displayed object on screen
     * - @code float angle@endcode › Angle of rotation of the object on screen
     * - @code float w, h@endcode › Editable width and height of the object (this actually determines the rendering)
     * - @code SDL_Color color@endcode › Color of the thing
     * - @code geng::Vertex pos@endcode › Pos of the transform
     * - @code geng::Vertex offset@endcode › How far off from center the object is rendered.
     * @warning Any negative value in a Transform object is not defined behavior within the engine. There are no safeguards.
     * @note In order for computationally-intensive Transform calculations like rotation to be used, the @code locked()@endcode flag must be set to @code false@endcode on the associated @code geng::Gear@endcode.
     * */
    struct Transform {
    private:
        /// Base width of the object
        uint16_t baseWidth = 10;
        /// Base height of the object
        uint16_t baseHeight = 10;
    public:
        /// Scale at which the object is rendered
        float scale = 1;
        /// Angle of rotation in degrees about the center
        float angle = 0;
        /// Editable width of the object
        float w = 10;
        /// Editable height of the object.
        float h = 10;
        /// Color of the transform.
        SDL_Color color = {255, 255, 255, 255};

        // Positioning
        /// Position of the Transform
        Vertex pos = Vertex(0,0,0);
        /// Rendering offset of the Transform
        Vertex offset = Vertex(0,0,0);

        /// Default constructor
        Transform() = default;
        /// Explicit constructor for position
        explicit Transform(Vertex pos) : pos(pos) {}
        /// Defines position, height, and width
        Transform(Vertex pos, uint16_t height, uint16_t width)
            : pos(pos), h(height), w(width), baseHeight(height), baseWidth(width) {}
        /// Defines position, height, width, and scale.
        Transform(Vertex pos, uint16_t height, uint16_t width, float scale)
            : pos(pos), h(height), w(width), baseHeight(height), baseWidth(width), scale(scale) {}

        // Resetting
        /// Returns width, height, and rotation to default values
        void reset() { w = baseWidth; h = baseHeight; angle = 0;}

        // Allows us to adjust posit

        std::string to_string() const {
            std::string result = "{\n";

            result += "  scale: " + std::to_string(scale) + ",\n";
            result += "  angle: " + std::to_string(angle) + ",\n";
            result += "  width: " + std::to_string(w) + ",\n";
            result += "  height: " + std::to_string(h) + ",\n";
            result += "  baseWidth: " + std::to_string(baseWidth) + ",\n";
            result += "  baseHeight: " + std::to_string(baseHeight) + ",\n";

            result += "  color: {r:" + std::to_string(color.r) +
                      ", g:" + std::to_string(color.g) +
                      ", b:" + std::to_string(color.b) +
                      ", a:" + std::to_string(color.a) + "},\n";
            result += "  pos: " + pos.to_string() + "\n";

            result += "}";
            return result;
        }

        // Gets the base width and height
        uint16_t get_base_width() const { return baseWidth; }
        uint16_t get_base_height() const { return baseHeight; }

        // Snap to region boundaries
        void snap_to_scene(EngineContext& world) {
            if (pos.x < 0)
                pos.x = 0;
            else if (pos.x >= world.get_width())
                pos.x = world.get_width() - 1;
            if (pos.y < 0)
                pos.y = 0;
            else if (pos.y >= world.get_height())
                pos.y = world.get_height() - 1;

        }
    };
}
