#pragma once

#include <cstdint>
#include <string>

// General editing
//#define PIXEL_PERFECT false
//#define LOAD_RENDER_STYLE "0"

namespace geng {
    /** Bitmask that contains several useful flags. The effect of each flag if "true" is given below:
     * - @code hidden@endcode › Does not render this object
     * - @code shadow@endcode › Render the shadow of this object
     * - @code flipX@endcode › Flip this object horizontally
     * - @code flipY@endcode › Flip this object vertically
     * - @code locked@endcode › Do not perform complicated render logic on this object, such as rotation
     * - @code tagged@endcode › Print this object's to_string each frame
     * - @code remove@endcode › Will remove this object on the next frame
     * - @code flipX@endcode › Flip this object horizontally
     * - @code flipY@endcode › Flip this object vertically
     * - @code draggable@endcode › Can be dragged around by the cursor
     * - @code dragged@endcode › This object is currently being dragged
     * - @code clicked@endcode › Object is currently clicked by the cursor
     * - @code actor@endcode › Means the object is an actor
     * - @code particle@endcode › Means the object is a particle
     * - @code banner@endcode › Means the object is a banner
     */
    enum class GFlag : uint16_t {
        none = 0,
        hidden = 1 << 0,    /// Not displayed by the renderer
        shadow = 1 << 1,    /// Shadow will not be displayed by the renderer
        locked = 1 << 4,    /// Prevents complex transformations from occurring
        tagged = 1 << 5,    /// Flags something to be printed by the engine per frame
        remove = 1 << 6,    /// Prompts the engine to remove this object
        flipX = 1 << 7,     /// Flips the target horizontally
        flipY = 1 << 8,     /// Flips the target vertically
        hovered = 1 << 9,     /// Hovering over the object will call it's on_hover() function
        draggable = 1 << 10,    /// Clicking on the object will call it's on_click() function
        dragged = 1 << 11,     /// This object can be dragged around by the cursor on click
        clicked = 1 << 12,  /// This object is currently is_clicked by the cursor. You do not manage this
        actor = 1 << 13,    /// Means that this object is an actor (DO NOT CHANGE)
        particle = 1 << 14, /// Means that this object is a particle (DO NOT CHANGE)
        banner = 1 << 15,    /// Means that this object is a banner (DO NOT CHANGE)
    };

    /// Returns true if the inner flag contains the outer flag.
    inline bool operator<<(GFlag f1, GFlag f2) {
        return (static_cast<uint16_t>(f1) & static_cast<uint16_t>(f2));
    }

    inline GFlag operator|(GFlag f1, GFlag f2) {
        return static_cast<GFlag>(static_cast<uint16_t>(f1) | static_cast<uint16_t>(f2));
    }

    inline void operator|=(GFlag& f1, GFlag f2) {
        f1 = static_cast<GFlag>(static_cast<uint16_t>(f1) | static_cast<uint16_t>(f2));
    }

    inline GFlag operator&(GFlag f1, GFlag f2) {
        return static_cast<GFlag>(static_cast<uint16_t>(f1) & static_cast<uint16_t>(f2));
    }

    inline void operator&=(GFlag& f1, GFlag f2) {
        f1 = static_cast<GFlag>(static_cast<uint16_t>(f1) & static_cast<uint16_t>(f2));
    }
    constexpr uint16_t GFLAG_ALL =
        static_cast<uint16_t>(GFlag::hidden) |
        static_cast<uint16_t>(GFlag::shadow) |
        static_cast<uint16_t>(GFlag::locked) |
        static_cast<uint16_t>(GFlag::tagged) |
        static_cast<uint16_t>(GFlag::remove) |
        static_cast<uint16_t>(GFlag::flipX) |
        static_cast<uint16_t>(GFlag::flipY) |
        static_cast<uint16_t>(GFlag::hovered) |
        static_cast<uint16_t>(GFlag::dragged) |
        static_cast<uint16_t>(GFlag::draggable) |
        static_cast<uint16_t>(GFlag::actor) |
        static_cast<uint16_t>(GFlag::particle) |
        static_cast<uint16_t>(GFlag::banner) |
        static_cast<uint16_t>(GFlag::clicked);

    inline GFlag operator~(GFlag a) {
        return static_cast<GFlag>(
            (~static_cast<uint16_t>(a)) & GFLAG_ALL
        );
    }

    /// Converts a GFlag into a string.
    inline std::string to_string(GFlag flag) {
        auto f = [](bool op) { return (op) ? "true" : "false"; };
        std::string ret = "Hidden: ";
        ret += f(flag << GFlag::hidden);
        ret += "\nShadow: ";
        ret += f(flag << GFlag::shadow);
        ret += "\nLocked: ";
        ret += f(flag << GFlag::locked);
        ret += "\nTagged: ";
        ret += f(flag << GFlag::tagged);
        ret += "\nRemove: ";
        ret += f(flag << GFlag::remove);
        ret += "\nClicked: ";
        ret += f(flag << GFlag::clicked);
        ret += "\nDragged: ";
        ret += f(flag << GFlag::dragged);
        return ret;
    }

    /* Render Frames */
    /**
     * @brief Describes what happens after each frame ends
     * - @code GAnim::IDLE@endcode › Stop at the end of the animation
     * - @code GAnim::RESET@endcode › Reset the animation to the default_animation
     * - @code GAnim::CONTINUE@endcode › Move to the next frame in memory
     * - @code GAnim::REPEAT@endcode › Repeat the current animation
     */
    enum class GAnimType : uint8_t {
        IDLE = 0,
        RESET = 1,
        CONTINUE = 2,
        REPEAT = 3
    };
}