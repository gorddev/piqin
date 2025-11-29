#pragma once
#include <cstdint>

namespace scene {
    // Scene width and height
    inline int width = 450;
    inline int height = 270;
    // Game timing (frame num, time, and dt)
    inline int frame = 0;
    inline double time = 0;
    inline double dt = 0;
    // Tells us whether we're in the main menu.
    inline bool mainManu = false;
    // Tells the renderer whether the fader is active or not
    inline bool fader = false;
    inline uint8_t faderAlpha = 80;
    inline uint8_t fadeStyle = 0;
}

// General editing
#define PIXEL_PERFECT true
#define LOAD_RENDER_STYLE "0"
#define IDLE 0

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
/* SHADOW CONSTANTS           */
#define SHADOW_ALPHA 50
#define SHADOW_OFFSET 5
/* FADING STYLES              */
#define FADE_INSTANT 0
#define FADE_SLOW 1
#define FADE_FAST 2

/* Objects Tags */
#define TAG_NONE 0
#define TAG_SELECTOR 1

/* Z-Indexing */
#define Z_MAX 100.0f