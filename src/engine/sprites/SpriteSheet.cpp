#include "engine/sprites/SpriteSheet.hpp"

#include <utility>

using namespace gengine;

/***************/
/* Constructors/Destructors */
// Constructor
SpriteSheet::SpriteSheet(SDL_Texture *newSheet)
	: texture(newSheet){
}

SpriteSheet::SpriteSheet(SDL_Texture *newSheet,
	std::unordered_map<uint8_t, std::vector<Frame>>
	newFrames) :
		texture(newSheet),
		frames(std::move(newFrames)) { }

SpriteSheet::SpriteSheet(int width, int height,
		int numColumns, int sprites, const std::string &p, GENG_Anim bhv, int dur) {
	frames[0].reserve(sprites);
	path = p;
	// Creates all of our frame goodies.
	for (int i = 0; i < sprites; i++) {
		int x = i*width % (numColumns*width);
		int y = (i*width / (numColumns*width)) * height;
		SDL_Rect box = {x, y, width, height};
		// We treat each frame as it's own state.
		frames[i].emplace_back(box, dur, bhv);
	}
}

// Destructor
SpriteSheet::~SpriteSheet() { }

/***************/
/* Getters and setters */
// Sets an animation
void SpriteSheet::set_texture(SDL_Texture *newSheet) {
	// Then we pipe in the new animation and new texture
	shadow = nullptr;
	texture = newSheet;
}

// Sets a custom shadow if needed
void SpriteSheet::set_shadow(SDL_Texture *newShadow) {
	shadow = newShadow;
}

// Gets the current animation
SDL_Texture* SpriteSheet::get_texture() const {
	return texture;
}
// Gets the current shadow
SDL_Texture* SpriteSheet::get_shadow() const {
	return shadow;
}

/***************/
/* Frame add/get */
void SpriteSheet::add_frames(std::vector<std::pair<int, std::vector<Frame>>> newFrames) {
	for (auto& [state, frameVec] : newFrames)
		frames[state] = frameVec;
}
void SpriteSheet::add_frames(int status_id, std::vector<Frame> newFrames) {
	// Here we move it because we'll never use this vector again.
	if (newFrames.size()>1) fixed = false;
	frames[status_id] = std::move(newFrames);
}

void SpriteSheet::add_frame(int status_id, Frame newFrame) {
	frames[status_id].emplace_back(newFrame);
}

SDL_Rect* SpriteSheet::get_frame(int state, int frame_id) {
	return &frames[state][frame_id].box;
}

std::string SpriteSheet::pop_path() {
	return path;
}

std::string SpriteSheet::to_string() {
	std::string ret = "Path: " + path + "\n";
	ret += "Texture Adr: " + std::to_string(reinterpret_cast<uint64_t>(texture)) + "\n";
	ret += "Shadow Adr: " + std::to_string(reinterpret_cast<uint64_t>(texture)) + "\n";
	ret += "Num frames: " + std::to_string(frames.size()) + "\n";
	for (auto& [state, frameVec] : frames) {
		ret += "x: " + std::to_string(frameVec[0].box.x);
		ret += ", y: " + std::to_string(frameVec[0].box.y);
		ret += ", w: " + std::to_string(frameVec[0].box.w);
		ret += ", h: " + std::to_string(frameVec[0].box.h) + "\n";
	}
	return ret;
}

void SpriteSheet::tick_frame(FrameState*& fs) {
	// If this object is not meant to change frame we just continue
	if (fs->duration == -1) return;
	// If we have a null state, we just return to beginning
	if (frames.find(fs->state) == frames.end()) {
		fs->state = fs->baseState;
		return;
	}
	// Otherwise, if it's equal to the duration we update the frameNum
	if (++fs->duration == frames[fs->state][fs->frameNum].duration) {
		// If we've reached the end of the frame we need to go back to start or go back to idle anim
		if (fs->frameNum++ >= frames[fs->state].size()) {
			// If stop, change our state to base state
			if (frames[fs->state][fs->frameNum - 1].nextFrame == GENG_Anim::STOP)
				fs->state = fs->baseState;
			// We always return to the start of the animation regardless
			fs->frameNum = 0;
			// And set our duration back to 0
			fs->duration = 0;
		}
	}
}


