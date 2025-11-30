#include <SDL.h>
#include <iostream>
#include <emscripten/html5.h>
#include <emscripten.h>

#include "Constants.hpp"
#include "EngineSource.hpp"
#include "game/GameMaster.hpp"
#include "utilities/BasisDecoder.hpp"

// Here's our renderer!
RenderManager* rend = nullptr;
// Here's our game manager!
GameMaster* gm = nullptr;
// They both have access to the entire game engine.

// gameloop defined below
EM_BOOL gameloop(double time, void* userdata) {

	// Time from previous events
	static double prevtime = 0;
	scene::frame++;
	scene::time = time;
	scene::dt = (time - prevtime);
	prevtime = time;

	(void)userdata;
	SDL_Event e;

	gm->blackjack();
	// This is our main loop.
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			return EM_FALSE;
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
			engine::input->update(e);
	}

	// Game master updates states of game logic and object paths
	gm->update();

	// Then we can finally render and present everything


	return EM_TRUE;
}


int main() {

	SDL_Init(SDL_INIT_VIDEO);

	// GameMaster -> Handles game logic
	gm = new GameMaster();
	gm->initialize();									// GameMaster

	// Now we just pray that the sheetManager piped the input properly. :o

	/************ FUCK AROUND ZONE**********/
	gm->add_card_to_hand({1,'s'});
	gm->add_card_to_hand({54,SPECIAL_CARD_SUIT});
	gm->add_card_to_hand({55,SPECIAL_CARD_SUIT});
	gm->add_card_to_hand({56,SPECIAL_CARD_SUIT});
	gm->add_card_to_hand({57,SPECIAL_CARD_SUIT});
	gm->add_card_to_hand({58,SPECIAL_CARD_SUIT});
	gm->add_card_to_hand({59,SPECIAL_CARD_SUIT});
	gm->set_hand_as_target();



	/******** END OF FUCK AROUND ZONE *******/
	emscripten_request_animation_frame_loop(gameloop, nullptr);
	emscripten_exit_with_live_runtime();

	// He's just a lil guy
	return 0;
}

