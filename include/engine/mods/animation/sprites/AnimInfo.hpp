#pragma once
#include "../frames/Frame.hpp"
#include "engine/layers/layer-subobjects/LayerState.hpp"

namespace gan {
    /**
     * @brief Contains all the information necessary to animate a @code ganine::Sprite@endcode 's sprite.
     * @details If you want a thorough understanding of AnimInfo, it is best to review how Frames and FrameTables function. However, we cover the fundamentals here:
     * 1. @code animation_index@endcode is the current animation the object is on. For example, "running" may be represented by the number 1, and "idle" may have a representation of 2.
     * 2. @code frame_index@endcode is the current frame of animation. For example, a "running" animation may have three frames, so frame_index will iterate from 0 to 2.
     * 3. @code texture_id@endcode links this AnimInfo to the @code TextureRegistry@endcode. You largely do not need to touch this.
     * 4. @code frame_sheet_id@endcode links this AnimInfo to a sheet of animations. For example, a "running" animation with 3 frames needs
     * to have the frame positions on the sprite texture stored in a FrameTable, and this variable keeps track of that FrameTable's ID.
     * 5. @code Frame* frame@endcode points to the current frame of animation. This is helpful to prevent deference chains during rendering.
     * You do not need to touch this, it is handled automatically.
     * 6. @code duration @endcode is the current duration of the animation in ms. Counts down from ANIM_Time to 0.
     * 7. @code default_animation@endcode. When a non-repeating animation ends, the AnimInfo returns to this animation.
     * 8. @code frameType@endcode is an enumerated uint8_t GENG_Anim type. Tells us what to do after this Frmae ends: @code IDLE, CONTINUE, REPEAT, STOP@endcode
     * You do not need to touch this after initializing a FrameTable.
     * 9. @code dirty@endcode AnimInfo flips this flag when the animation_index changes or duration expires. This signals the engine to grab a new frame for the AnimInfo.
     * You do not need to touch this.
     * @warning When making @code ganine::Sprite@endcode objects with AnimInfos, you must initialize AnimInfo values before calling engine.add_sprite(...). Otherwise, this will result in undefined behavior.
     */
    struct AnimInfo {
    private:
        /// Frame we are currently on.
        Frame* frame;
        /// The current animation the sprites is on.
        uint16_t animation_index = 0;
        /// The next animation to head to
        short next_anim = -1;
        /// A queued animation
        short queued_anim = -1;
        /// The current frame of the animation the sprites is on.
        uint16_t frame_index = 0;
        /// The default animation for this sprites
        uint16_t default_animation = 0;
        /// Determines if the current animation has priority against suggestion
        bool priority = false;
        /// Running duration of the animation
        float duration = 0.f;
    public:
        /// Flag for engine to update the frame. Do not touch in a hot loop unless you know what you're doing.
        bool dirty = false;

        /// Basic constructor for animInfo without default frame
        explicit AnimInfo(Frame& first_frame);
        /// Complete constructor for AnimInfo
        AnimInfo(Frame& first_frame, uint16_t default_animation);

        /// Reduces the duration by dt, and returns true if the frame needs to be updated.
        bool update(LayerState &time);

        // Getters
        /// Returns a copy of the animation_index of the AnimInfo
        [[nodiscard]] uint16_t get_anim_id() const;

        short get_next_anim() const;

        void set_next_anim(short new_anim);

        /// Returns a copy of the frame index
        [[nodiscard]] uint16_t get_frame_index() const;
        /// Returns a copy of the frame type.
        [[nodiscard]] GAnimType get_frame_type() const;
        /// Gets the default animation of the anim_info object
        [[nodiscard]] uint16_t get_default_animation() const;

        void set_frame_id(uint16_t new_frame_id);

        // Setters
        /// Sets the frame of the AnimInfo
        void set_frame(Frame& s);
        /// Sets the animation for the AnimInfo, and flags the engine to change the frame.
        void set_animation(uint16_t new_animation, bool priorityStatus = false);
        /// Sets the animation to start as soon as the current frame expires
        void suggest_animation(short new_animation);
        /// Queues up an animation to start once the current one is finished.
        void queue_animation(short new_animation);

        void set_priority(bool state);

        bool has_priority();

        /// Sets the default animation
        void set_default_animation(uint16_t new_default_animation);

        short get_queued_anim() const;

        // Utility
        /// Appends buffer to the buffer
        void calc_vertices(RenderBuffer& buffer, Gear* gear);
        /// Increments the frame_index by one, and returns the new frame_index
        int pre_increment_frame();
        /// Debugger printer for AnimInfo verbose style
        [[nodiscard]] gan::str_view& to_fstring_verbose(gan::str_view& buffer) const;
        /// Debugger printer for AnimInfo short style
        [[nodiscard]] str_view& to_fstring(gan::str_view& buffer) const;
    };
}
