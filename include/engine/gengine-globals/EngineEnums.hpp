#pragma once

// General editing
#define PIXEL_PERFECT true
#define LOAD_RENDER_STYLE "0"

/* ************************** */
/* SHADOW CONSTANTS           */
#define SHADOW_ALPHA 50
#define SHADOW_OFFSET 5

/* Z-Indexing */
#define Z_MAX 100.0f

namespace gengine {
    /* FADING STYLES              */
    enum class GENG_Fade : uint8_t {
        INSTANT = 0,
        SLOW = 1,
        FAST = 2
    };
    /* Objects Tags */
    enum class GENG_Tag: uint8_t {
        NONE = 0,
        SELECTOR = 1
    };
    /* RENDER Z-SORTING */
    enum class GENG_Sort : uint8_t {
        NONE = 0,
        ASCENDING_BATCH = 1,
        DESCENDING_BATCH = 2,
        ASCENDING = 3,
        DESCENDING = 4
    };
    /* Render Frames */
    enum class GENG_Anim : uint8_t {
        IDLE = 0,
        RESET = 1,
        CONTINUE = 2,
        REPEAT = 3
    };
    enum class GENG_Type : uint8_t {
        OBJECT = 0,
        PARTICLE = 1,
        UI = 2
    };
}