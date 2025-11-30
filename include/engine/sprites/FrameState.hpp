#pragma once
#include <cstdint>

namespace gengine {
    struct FrameState {
        // Information about frames
        int frameNum = 0;
        // State gives current animation they are on
        uint8_t state = 0;
        // gives the idle state of a sprite
        uint8_t baseState = 0;
        // Sheet ID
        int sheet_id = -1;
        // Duration is the duration of the frame
        float duration = 0;

        FrameState() = default;
        FrameState(int sheet_id) : sheet_id(sheet_id) {}
    };
}
