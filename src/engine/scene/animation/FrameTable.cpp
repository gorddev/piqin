#include "engine/scene/animation/FrameTable.hpp"
#include "engine/scene/animation/AnimInfo.hpp"
#include <utility>

#include "../../../../include/engine/debug/geng_debug.hpp"

using namespace gan;

FrameTable::FrameTable(gch::vector<gch::vector<AnimBox2D>> quadVec) {
    quads = std::move(quadVec);
}

FrameTable::FrameTable(int startx, int starty, int w, int h,
            short numColumns, short numAnimations) {
    quads.reserve(numAnimations);
    int count = 0;
    for (int k = 0; count < numAnimations; k++) {
        for (int i = 0; i < numColumns && count < numAnimations; i++) {
            AnimBox2D q(startx + (w*i), starty + (k*h), w, h);
            quads.push_back({q});
            count++;
        }
    }
}

FrameTable::FrameTable(int startx, int starty,
        int w, int h, short numColumns, short numAnimations,
        gch::vector<std::pair<int, gch::vector<AnimBox2D>>>& overrides)
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
        glog::dev << "<" << q.size() << "> ";
    }
    glog::err.src("FrameTable::get_frame()") << "Frame " << frameNum << " of animation " << animationNum << " not found." << glog::endlog;
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

