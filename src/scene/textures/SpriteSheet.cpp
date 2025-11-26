#include "textures/SpriteSheet.hpp"

#include <utility>
#include "Constants.hpp"

/***************/
/* Constructors/Destructors */
// Constructor
SpriteSheet::SpriteSheet(SDL_Texture *newSheet, uint8_t newRenderType)
	: texture(newSheet), renderType(newRenderType) {
}

SpriteSheet::SpriteSheet(SDL_Texture *newSheet,
	uint8_t newRenderType,
	std::unordered_map<uint8_t, std::vector<Frame>>
	newFrames) :
		texture(newSheet),
		renderType(newRenderType),
		frames(std::move(newFrames)) { }

SpriteSheet::SpriteSheet(int width, int height,
		int numColumns, int sprites, const string &p,
		const uint8_t renderT, uint8_t bhv, int dur) {
	renderType = renderT;
	frames[bhv].reserve(sprites);
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
// Sets an animation with a new render type
void SpriteSheet::set_texture(SDL_Texture *newSheet, const uint8_t newRenderType) {
	shadow = nullptr;
	renderType = newRenderType;
	// Calls the original function
	set_texture(newSheet);
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

uint8_t SpriteSheet::get_renderType() const {
	return renderType;
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

string SpriteSheet::pop_path() {
	return path;
}

string SpriteSheet::to_string() {
	string ret = "Path: " + path + "\n";
	ret += "Texture Adr: " + std::to_string(reinterpret_cast<uint64_t>(texture)) + "\n";
	ret += "Shadow Adr: " + std::to_string(reinterpret_cast<uint64_t>(texture)) + "\n";
	ret += "Num frames: " + std::to_string(frames.size()) + "\n";
	ret += "Render Type: " + std::to_string(renderType) + "\n";
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
		fs->state = IDLE;
		return;
	}
	// Otherwise, if it's equal to the duration we update the frameNum
	if (++fs->duration == frames[fs->state][fs->frameNum].duration) {
		// If we've reached the end of the frame we need to go back to start or go back to idle anim
		if (fs->frameNum++ >= frames[fs->state].size()) {
			// If stop, change our state to idle
			if (frames[fs->state][fs->frameNum - 1].nextFrame == FRAME_STOP)
				fs->state = IDLE;
			// We always return to the start of the animation regardless
			fs->frameNum = 0;
			// And set our duration back to 0
			fs->duration = 0;
		}
	}
}


