#pragma once
#include "Frame.hpp"
#include "engine/scene/layers/LayerTime.hpp"

namespace geng {
    /**
     * @brief Contains all the information necessary to animate a @code gengine::Actor@endcode 's sprite.
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
     * @warning When making @code gengine::Actor@endcode objects with AnimInfos, you must initialize AnimInfo values before calling engine.add_actor(...). Otherwise, this will result in undefined behavior.
     */
    struct AnimInfo {
    private:
        /// Frame we are currently on.
        Frame* frame = nullptr;
        /// The current animation the sprite is on.
        uint16_t animation_index = 0;
        /// The current frame of the animation the sprite is on.
        uint16_t frame_index = 0;
        /// ID of the sprite's texture
        uint16_t texture_id = 0;
        /// ID of the sprite's FrameTable
        uint16_t frame_table_id = 0;
    public:
        /// Duration
        float duration = 0.f;
        /// The default animation for this sprite
        uint16_t default_animation = 0;
        /// Current animation type
        GAnimType frameType = GAnimType::IDLE;
        /// Flag for engine. Do not touch in a hot loop unless you know what you're doing.
        bool dirty = false;


        /// Default constructor for AnimInfo. Should not be used willy-nilly.
        AnimInfo() = default;
        /// Complete constructor for AnimInfo
        AnimInfo(uint16_t texture_id, uint16_t frame_sheet_id, uint16_t animation_index);

        /// Reduces the duration by dt, and returns true if the frame needs to be updated.
        bool update(LayerTime &time);

        // Getters
        /// Returns a copy of the animation_index of the AnimInfo
        [[nodiscard]] uint16_t get_anim_id() const;
        /// Returns a copy of the frame index
        [[nodiscard]] uint16_t get_frame_index() const;
        /// Returns a copy of the frame_table_id
        [[nodiscard]] uint16_t get_frame_table_id() const;
        /// Returns a copy of the texture_sheet_id
        [[nodiscard]] uint16_t get_texture_id() const;

        // Setters
        /// Sets the frame of the AnimInfo
        void set_frame(Frame& s);
        /// Sets the animation for the AnimInfo, and flags the engine to change the frame.
        void set_animation(uint16_t new_animation);
        /// Sets the frame sheet required
        void set_frame_table_id(uint16_t new_frame_table_id);

        // Utility
        /// Appends buffer to the buffer
        void calc_vertices(RenderBuffer& buffer, Gear* gear);
        /// Increments the frame_index by one, and returns the new frame_index
        int pre_increment_frame();
        /// Debugger printer for AnimInfo.
        [[nodiscard]] std::string to_string() const;
    };
}
