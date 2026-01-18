#pragma once

#include "../../debug/geng_debug.hpp"
#include "../../types/strings/fstring/fstring.hpp"

// General editing
//#define PIXEL_PERFECT false
//#define LOAD_RENDER_STYLE "0"

namespace gan {
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
     * - @code sprite@endcode › Means the object is an sprite
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
        sprite = 1 << 13,    /// Means that this object is an sprites (DO NOT CHANGE)
        particle = 1 << 14, /// Means that this object is a particle (DO NOT CHANGE)
        banner = 1 << 15,    /// Means that this object is a banner (DO NOT CHANGE)
    };

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
        static_cast<uint16_t>(GFlag::sprite) |
        static_cast<uint16_t>(GFlag::particle) |
        static_cast<uint16_t>(GFlag::banner) |
        static_cast<uint16_t>(GFlag::clicked);

    inline GFlag operator~(GFlag a) {
        return static_cast<GFlag>(
            (~static_cast<uint16_t>(a)) & GFLAG_ALL
        );
    }

    /// Converts a GFlag into a verbose string.
    inline gan::str_view& to_fstring_verbose(gan::str_view& buffer, GFlag flag) {
        auto f = [](GFlag op) { return (static_cast<bool>(op)) ? "true" : "false"; };
        buffer << "type: ";
        buffer << (static_cast<bool>(flag & GFlag::banner) ? "[e]banner[n] " : static_cast<bool>(flag & GFlag::sprite) ? "[e]sprite[n] " : "[e]particle[n] ");
        buffer << "\tvis: ";
        buffer << f((~flag & GFlag::hidden));
        buffer << "\tsdw: ";
        buffer << f(flag & GFlag::shadow);
        buffer << "\tlock: ";
        buffer << f(flag & GFlag::locked);
        buffer << "\ttag: ";
        buffer << f(flag & GFlag::tagged);
        buffer << "\nhvr: ";
        buffer << f(flag & GFlag::hovered);
        buffer << "\tclk: ";
        buffer << f(flag & GFlag::clicked);
        buffer << "\tdrgble: ";
        buffer << f(flag & GFlag::draggable);
        buffer << "\tdragged: ";
        buffer << f(flag & GFlag::dragged);
        return buffer;
    }

    /// Converts a GFlag into a short string.
    inline gan::str_view& to_fstring(gan::str_view& buffer, GFlag flag) {
        auto f = [](GFlag op) { return (static_cast<bool>(op)) ? "true" : "false"; };
        buffer << "type: ";
        buffer << (static_cast<bool>(flag & GFlag::banner) ? "[e]banner[n] " : static_cast<bool>(flag & GFlag::sprite) ? "[e]sprite[n] " : "[e]particle[n] ");
        buffer << "\tsdw: ";
        buffer << f(flag & GFlag::shadow);
        buffer << "\tclk: ";
        buffer << f(flag & GFlag::clicked);
        return buffer;
    }

    /* Render Frames */

}