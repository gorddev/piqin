#pragma once
#include <SDL.h>

// Each frame has a box and a duration.
struct Frame {
	SDL_Rect box;
	int duration;
	uint8_t nextFrame;
	Frame(const SDL_Rect &box, const int duration, const uint8_t nextFrame) :
		box(box), duration(duration), nextFrame(nextFrame) {}
};