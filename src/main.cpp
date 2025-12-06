#include <SDL.h>
#include <iostream>
#include <emscripten/html5.h>
#include <emscripten.h>

#include "EngineSource.hpp"
#include "game/GameMaster.hpp"
#include "engine/textures/asset-info/HeaderRegistry.hpp"

// Here's our game manager!
GameMaster* gm = nullptr;

// gameloop defined below
EM_BOOL gameloop(double time, void* userdata) {
	(void)userdata;
	// <><><><><><><><>
	// Updates our time and grabs user input & runs events
	// <><><><><><><><>
	static int x = 200;

	if (!bob.tick(time))
		return EM_FALSE;
	double newtime = emscripten_get_now();

	// All game logic goes here!
	gm->blackjack();
	// Game master updates states of game logic and object paths
	gm->update();
	newtime = emscripten_get_now();

	// <><><><><><><><>
	// Finally we render
	// <><><><><><><><>
	bob.render();

	return EM_TRUE;
}


int main() {
	bob.initialize();

	// GameMaster -> Handles game logic
	gm = new GameMaster();
	gm->initialize();									// GameMaster
	// Now we just pray that the sheetManager piped the input properly. :o

	/************ FUCK AROUND ZONE**********/

	gm->add_card_to_hand({7, blackjack::BJ_Suit::HEART});
	gm->add_card_to_hand({54, blackjack::BJ_Suit::SPECIAL});
	gm->add_card_to_hand({55, blackjack::BJ_Suit::SPECIAL});
	gm->add_card_to_hand({56, blackjack::BJ_Suit::SPECIAL});
	gm->add_card_to_hand({57, blackjack::BJ_Suit::SPECIAL});
	gm->add_card_to_hand({58, blackjack::BJ_Suit::SPECIAL});
	gm->add_card_to_hand({59, blackjack::BJ_Suit::SPECIAL});
	gm->set_hand_as_target();

	/******** END OF FUCK AROUND ZONE *******/
	emscripten_request_animation_frame_loop(gameloop, nullptr);
	emscripten_exit_with_live_runtime();

	// He's just a lil guy
	return 0;
}

