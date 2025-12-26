#pragma once

#include "../../core/gears/GearFlag.hpp"
#include "../../core/gears/Gear.hpp"
#include "../../types/AnimBox2D.hpp"
#include "engine/utilities/image-info/IMGDecoder.hpp"

namespace geng {
    /**
     * @brief Holds the points from a texture necessary to render a single sprite at render time. Usually, you do not make Frames manually,
     * and form them from a @code gengine::Quad@endcode with the @code Quad.to_frame()@endcode function. Quads can be found in engine/types.
     * @details Frames contain key member variables and functions to speed up calculations during render time.
     * - @code lni::vector<SDL_FPoint> vertexPoints@endcode › contains the points on the Texture where the frame's image data is stored.
     * - @code float duration@endcode › The duration of the frame itself in ms.
     * - @code GAnim anim@endcode › Contains what to do when the Frame ends (REPEAT, IDLE, CONTINUE, RESET)
     * - @code append_vertices(...)@endcode › Appens the correct vertices directly to the render buffer,
     * with proper rotation, stretching, ect. adjustments applied.
     * */
    struct Frame {
    private:
        /// Holds the points on a FPos2D necessary for rendering.
        gch::vector<SDL_FPoint> vertexPoints;
        /// Base duration of the frame.
        float duration;
        /// What we do after this animation ends (CONTINUE, STOP, IDLE, REPEAT)
        GAnimType anim = GAnimType::IDLE;
        /// If we have a specific next animation after this one (overrides GAnimType)
        short next_anim = -1;
    public:
        Frame() = default;
        /// Constructs a frame from a vector of points, a duration, and an animation.
        Frame(AnimBox2D, IMG_Info& info);
        /// Sends the buffer to the renderer for rendering. Performs all the calculations for rotation, scaling, ect.
        void append_vertices(RenderBuffer &buffer, Gear *g);

        // Getters
        /// Returns the duration of the frame
        [[nodiscard]] float get_duration() const;
        /// Returns the animation type of the frame.
        [[nodiscard]] GAnimType get_anim_type() const;
        /// Returns the next animation
        [[nodiscard]] short get_next_anim() const;
    };
}
