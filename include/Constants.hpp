#pragma once

namespace scene {
    inline int width = 480;
    inline int height = 270;
    inline int frame = 0;
    inline double time = 0;
    inline double dt = 0;
    inline bool mainManu = false;
}

#define PIXEL_PERFECT true
#define LOAD_RENDER_STYLE "0"

// Velocity Scale (how much velocity impacts movement)
#define VEL_SCALE 0.1

/* State of Objects */
// All Objects
#define IDLE 0
#define STATIC 99

/* ************************** */
/* FRAME DATA and FRAME SHEETS*/
/* CONSTANTS for Frame States */
#define FRAME_STOP 0
#define FRAME_CONTINUE 1
#define FRAME_REPEAT 2

/* ************************** */
/* RENDERING STYLES           */
#define RENDER_FLAT 0
#define RENDER_VERTICAL 1
#define RENDER_UI 2

/* RENDERING CONSTANTS      */
#define ZTOP 500
#define SHADOW_ALPHA 50
#define SHADOW_OFFSET 5

/* Z-INDEXING */
#define Z_HAND_BASE 10