#pragma once
#include <cstdint>
#include "Constants.hpp"

struct FrameState {
    // Information about frames
    int frameNum = 0;
    // State gives current animation they are on
    uint8_t state = IDLE;
    // gives the idle state of a sprite
    uint8_t baseState = IDLE;
    // Sheet ID
    int sheet_id = -1;
    // Duration is the duration of the frame
    float duration = 0;

    FrameState() = default;
    FrameState(int sheet_id) : sheet_id(sheet_id) {}
};
