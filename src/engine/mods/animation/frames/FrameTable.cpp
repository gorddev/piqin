#include "engine/mods/animation/frames/FrameTable.hpp"
#include "engine/mods/animation/sprites/AnimInfo.hpp"
#include <utility>

#include "engine/debug/geng_debug.hpp"

using namespace gan;

FrameTable::FrameTable(std::vector<std::vector<AnimBox2D>> quads) {
    this->quads = std::move(quads);
}

FrameTable::FrameTable(const int start_x, const int starty, int w, int h,
            const short numColumns, const short numAnimations, const short padding) {
    quads.reserve(numAnimations);
    int count = 0;
    int pad_y = padding;
    for (int k = 0; count < numAnimations; k++) {
        int pad_x = padding;
        for (int i = 0; i < numColumns && count < numAnimations; i++) {
            AnimBox2D q(start_x + (w*i) + pad_x, starty + (k*h) + pad_y, w, h);
            quads.push_back({q});
            count++;
            pad_x += padding;
        }
        pad_y += padding;
    }
}

FrameTable::FrameTable(int startx, int starty,
        int w, int h, short numColumns, short numAnimations,
        std::vector<std::pair<int, std::vector<AnimBox2D>>>& overrides)
{
    quads.resize(numAnimations);
    size_t ovr = 0;
    for (int anim = 0; anim < numAnimations; ++anim) {

        if (ovr < overrides.size() && overrides[ovr].first == anim) {
            // Explicit override frames
            for (const AnimBox2D& q : overrides[ovr].second)
                quads[anim].push_back(q);
            ++ovr;
        }
        else {
            int col = anim % numColumns;
            int row = anim / numColumns;
            AnimBox2D q(startx + col * w, starty + row * h, w, h);
            quads.push_back({q});
        }
    }
}

void FrameTable::_init(IMG_Info& info) {
    frames.resize(quads.size());
    for (int anim = 0; anim < static_cast<int>(quads.size()); anim++) {
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
    if (animationNum < static_cast<int>(frames.size()) && frameNum < static_cast<int>(frames[animationNum].size())) {
        return frames[animationNum][frameNum];
    }

    for (auto& q : frames) {
        glog::dev << "<" << q.size() << "> ";
    }
    glog::err.src("FrameTable::get_frame()") << "Frame " << frameNum << " of animation " << animationNum << " not found." << glog::endlog;
    return frames[animationNum][frameNum];
}


void FrameTable::update_frame(AnimInfo& s) {
    if (!s.has_priority() && s.get_next_anim() != -1 && s.get_next_anim() != s.get_anim_id()) {
        s.set_animation(s.get_next_anim());
        s.set_frame(frames[s.get_anim_id()][0]);
    }
    else if (s.get_frame_type()== GAnimType::IDLE) {
        s.set_priority(false);
        s.set_frame(frames[s.get_anim_id()][s.get_frame_index()]);
    }
    else if (s.get_frame_type() == GAnimType::CONTINUE) {
        s.pre_increment_frame();
        s.set_frame(frames[s.get_anim_id()][s.get_frame_index()]);
    }
    else if (s.get_frame_type() == GAnimType::RESET) {
        if (s.get_queued_anim() >= 0) {
            s.set_animation(s.get_queued_anim());
            s.queue_animation(-1);
            s.set_frame(frames[s.get_anim_id()][0]);
        }
        else {
            s.set_frame_id(0);
            if (s.get_next_anim() < 0)
                s.set_animation(s.get_default_animation());
            else
                s.set_animation(s.get_next_anim());
            s.set_frame(frames[s.get_anim_id()][0]);
        }
    }
    else if (s.get_frame_type() == GAnimType::REPEAT) {
        s.set_frame(frames[s.get_anim_id()][0]);
        s.set_frame_id(0);
    }
    s.dirty = false;
}

