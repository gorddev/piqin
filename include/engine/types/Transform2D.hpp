#pragma once
#include "positioning/FPos2D.hpp"
#include "engine/EngineContext.hpp"
#include <SDL.h>
#include <string>

#include "engine/layers/LayerContext.hpp"

namespace geng {

/** @brief The fundamental target for all morphs, routes, shaking, and rendering.
 * @details The Transform2D contains all the position information necessary for a game object to be rendered. Although @code geng::Gear@endcode s are the primary
 * holders of transforms, other object types, like user-defined subclasses of @code geng::Mesh@endcode can also hold transforms. We briefly cover
 * how the engine interfaces with @code geng::Transform2D@endcode.
 * - @code engine.render()@endcode Relies on the width, height, position, and offset within a transform to render @code geng::Sprites@endcode
 * - @code geng::FPos2D pos@endcode and @code geng::FPos2D offset@endcode perform two different functions. @code pos@endcode keeps track of the absolute position
 * - Member variables below:
 * - @code uint16_t baseWidth, baseHeight@endcode › Base width and height of the object: private.
 * - @code float scale@endcode › scale of the displayed object on screen
 * - @code float angle@endcode › Angle of rotation of the object on screen
 * - @code float w, h@endcode › Editable width and height of the object (this actually determines the rendering)
 * - @code SDL_Color color@endcode › Color of the thing
 * - @code geng::FPos2D pos@endcode › Pos of the transform
 * - @code geng::FPos2D offset@endcode › How far off from center the object is rendered.
 * @warning Any negative value in a Transform2D object is not defined behavior within the engine. There are no safeguards.
 * @note In order for computationally-intensive Transform2D calculations like rotation to be used, the @code locked()@endcode flag must be set to @code false@endcode on the associated @code geng::Gear@endcode.
 */
struct Transform2D {
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
    /// Position of the Transform2D
    FPos2D pos = {0,0};
    /// Rendering offset of the Transform2D
    FPos2D offset = {0,0};

    /// Default constructor
    Transform2D();
    /// Explicit constructor for position
    explicit Transform2D(FPos2D pos);
    /// Defines position, height, and width
    Transform2D(FPos2D pos, uint16_t width, uint16_t height);
    /// Defines position, height, width, and scale.
    Transform2D(FPos2D pos, uint16_t width, uint16_t height, float scale);

    /// Returns the adjust position with width and height of the transform
    [[nodiscard]] FPos2D get_s_pos();
    /// Sets the adjusted position of the transform
    void set_s_pos(FPos2D new_pos);

    // Resetting
    /// Returns width, height, and rotation to default values
    void reset();

    /// Converts object to string representation for debugging
    std::string to_string() const;

    /// Gets the base width and height
    uint16_t get_base_width() const;
    uint16_t get_base_height() const;

    /// Snap to region boundaries
    void snap_to_scene(LayerContext& scene);

    /// Gets the points to render the hitbox of the transform object.
    std::vector<SDL_FPoint> to_vertex_hitbox(
        uint16_t thickness);
};

} // namespace geng