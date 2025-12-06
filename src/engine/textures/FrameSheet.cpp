#include "engine/textures/FrameSheet.hpp"

#include <iostream>

#include "engine/textures/FrameState.hpp"
#include "engine/textures/asset-info/TextureRegister.hpp"

using namespace gengine::textures;

FrameSheet::FrameSheet() : texture_id(0), frames(0) {}

FrameSheet::FrameSheet(std::string path, int texWidth, int texHeight, std::vector<std::vector<Quad>> quads) {
    texture_id = tex_register.register_texture(path);
    frames.resize(quads.size());
    for (int i = 0; i < quads.size(); i++) {
        for (auto q : quads[i]) {
            frames[i].push_back(q.to_frame(texWidth, texHeight));
        }
    }
}

FrameSheet::FrameSheet(std::string path, int texWidth, int texHeight, int startx, int starty, int w, int h,
            short numColumns, short numFrames) {
    std::cerr << "trying to register texture" << path << std::endl;

    texture_id = textures::tex_register.register_texture(path);
    std::cerr << "texwidth: " << texWidth << std::endl;
    std::cerr << "texheight: " << texHeight << std::endl;
    std::cerr << "startx: " << startx << std::endl;
    std::cerr << "starty: " << starty << std::endl;
    std::cerr << "w: " << w << std::endl;
    std::cerr << "h: " << h << std::endl;
    std::cerr << "numcolumns: " << numColumns << std::endl;
    std::cerr << "numframes: " << numFrames << std::endl;
    frames.resize(numFrames);
    int count = 0;
    for (int k = 0; count < numFrames; k++) {
        for (int i = 0; i < numColumns && count < numFrames; i++) {
            Quad q(startx + (w*i), starty + (k*h), w, h);
            frames[count].push_back(q.to_frame(texWidth, texHeight));
            count++;
        }
    }
    std::cerr << "exit?\n";
}

FrameSheet::FrameSheet(std::string path, int texWidth, int texHeight,
        int startx, int starty,
        int w, int h, short numColumns, short numFrames,
        const std::vector<std::pair<int, std::vector<Quad>>> overrides)
{
    texture_id = tex_register.register_texture(path);
    frames.resize(numFrames);

    int ovr = 0; // start index of override vector
    int frameNum = 0;

    for (int count = 0; count < numFrames; count++) {
        if (ovr < overrides.size() && overrides[ovr].first == count) {
            for (auto& q : overrides[ovr].second) {
                frames[frameNum].push_back(q.to_frame(texWidth, texHeight));
            }
            ovr++;
        }
        else {
            int col = frameNum % numColumns;
            int row = frameNum / numColumns;
            Quad q(startx + w*col, starty + h*row, w, h);
            frames[frameNum].push_back(q.to_frame(texWidth, texHeight));
        }
        frameNum++;
    }
}


int FrameSheet::get_texture_id() {
    return texture_id;
}

Frame& FrameSheet::get_frame(int animationNum, int frameNum) {
    if (animationNum < frames.size() && frameNum < frames[animationNum].size()) {
        return frames[animationNum][frameNum];
    }
    std::cerr << "FrameSheet::frame [" << animationNum << "][" << frameNum << "] does not exist. aborting.\n";
    abort();
}

void FrameSheet::update_frame(FrameState& s) {
    if (s.frameType== GENG_Anim::IDLE)
        s.set_frame(frames[s.animation_index][0]);
    if (s.frameType == GENG_Anim::CONTINUE)
        s.set_frame(frames[s.animation_index][++s.frame_index]);
    else if (s.frameType == GENG_Anim::RESET)
        s.set_frame(frames[s.default_animation][0]);
    else if (s.frameType == GENG_Anim::REPEAT)
        s.set_frame(frames[s.animation_index][++s.frame_index]);
}
