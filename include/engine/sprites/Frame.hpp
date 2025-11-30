#pragma once

// Each frame has a box and a duration.
namespace gengine {
	struct Frame {
		SDL_Rect box;
		int duration;
		GENG_Anim nextFrame;
		Frame(const SDL_Rect &box, const int duration, const GENG_Anim nextFrame) :
			box(box), duration(duration), nextFrame(nextFrame) {}
	};
}