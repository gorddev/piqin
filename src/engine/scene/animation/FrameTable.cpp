#include "engine/scene/animation/FrameTable.hpp"
#include "engine/scene/animation/AnimInfo.hpp"
#include <iostream>
#include <utility>

using namespace geng;

FrameTable::FrameTable(std::vector<std::vector<Quad>> quadVec) {
    quads = std::move(quadVec);
}

FrameTable::FrameTable(int startx, int starty, int w, int h,
            short numColumns, short numAnimations) {
    quads.reserve(numAnimations);
    int count = 0;
    for (int k = 0; count < numAnimations; k++) {
        for (int i = 0; i < numColumns && count < numAnimations; i++) {
            Quad q(startx + (w*i), starty + (k*h), w, h);
            quads.push_back({q});
            count++;
        }
    }
}

FrameTable::FrameTable(int startx, int starty,
        int w, int h, short numColumns, short numAnimations,
        const std::vector<std::pair<int, std::vector<Quad>>>& overrides)
{
    quads.resize(numAnimations);
    size_t ovr = 0;
    for (int anim = 0; anim < numAnimations; ++anim) {

        if (ovr < overrides.size() && overrides[ovr].first == anim) {
            // Explicit override frames
            for (const Quad& q : overrides[ovr].second)
                quads[anim].push_back(q);
            ++ovr;
        }
        else {
            int col = anim % numColumns;
            int row = anim / numColumns;
            Quad q(startx + col * w, starty + row * h, w, h);
            quads.push_back({q});
        }
    }
}

void FrameTable::_init(IMG_Info& info) {
    frames.resize(quads.size());
    for (int anim = 0; anim < quads.size(); anim++) {
        for (auto& q2: quads[anim]) {
            frames[anim].emplace_back(q2, info);
        }
    }
    // Clears out the memory of the quads.
    quads = {};
}

int FrameTable::get_texture_id() const {
    return texture_id;
}

void FrameTable::set_texture_id(int id) {
    texture_id = id;
}

Frame& FrameTable::get_frame(int animationNum, int frameNum) {
    if (animationNum < frames.size() && frameNum < frames[animationNum].size()) {
        return frames[animationNum][frameNum];
    }

    for (auto& q : frames) {
        std::cerr << "<" << q.size() << "> ";
    }
    std::cerr << std::endl;
    abort();
}


void FrameTable::update_frame(AnimInfo& s) {
    if (s.get_frame_type()== GAnimType::IDLE)
        s.set_frame(frames[s.get_anim_id()][0]);
    if (s.get_frame_type() == GAnimType::CONTINUE)
        s.set_frame(frames[s.get_anim_id()][s.pre_increment_frame()]);
    else if (s.get_frame_type() == GAnimType::RESET)
        s.set_frame(frames[s.get_default_animation()][0]);
    else if (s.get_frame_type() == GAnimType::REPEAT)
        s.set_frame(frames[s.get_anim_id()][s.pre_increment_frame()]);
    s.dirty = false;
}

