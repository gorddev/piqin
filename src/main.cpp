#include <SDL.h>
#include <iostream>
#include <emscripten/html5.h>
#include <emscripten.h>

#include "EngineSource.hpp"
#include "game/GameMaster.hpp"

// Here's our game manager!
GameMaster* gm = nullptr;

// gameloop defined below
EM_BOOL gameloop(double time, void* userdata) {
	(void)userdata;
	// <><><><><><><><>
	// Updates our time and grabs user input & runs events
	// <><><><><><><><>
	if (!bob.tick(time))
		return EM_FALSE;

	// All game logic goes here!
	gm->blackjack();

	// Game master updates states of game logic and object paths
	gm->update();

	// <><><><><><><><>
	// Finally we render
	// <><><><><><><><>
	bob.render();

	return EM_TRUE;
}


int main() {
	std::cerr << "spot 0\n";
	bob.initialize();

	std::cerr << "spot 1\n";
	// GameMaster -> Handles game logic
	gm = new GameMaster();
	gm->initialize();									// GameMaster

	std::cerr << "spot 2\n";
	// Now we just pray that the sheetManager piped the input properly. :o

	/************ FUCK AROUND ZONE**********/

	gm->add_card_to_hand({7, blackjack::BJ_Suit::HEART});
	gm->add_card_to_hand({54, blackjack::BJ_Suit::SPECIAL});
	gm->add_card_to_hand({55, blackjack::BJ_Suit::SPECIAL});
	gm->add_card_to_hand({56, blackjack::BJ_Suit::SPECIAL});
	gm->add_card_to_hand({57, blackjack::BJ_Suit::SPECIAL});
	gm->add_card_to_hand({58, blackjack::BJ_Suit::SPECIAL});
	gm->add_card_to_hand({59, blackjack::BJ_Suit::SPECIAL});
	std::cerr << "spot 3\n";
	gm->set_hand_as_target();

	std::cerr << "spot 4\n";

	/******** END OF FUCK AROUND ZONE *******/
	emscripten_request_animation_frame_loop(gameloop, nullptr);
	emscripten_exit_with_live_runtime();

	// He's just a lil guy
	return 0;
}

