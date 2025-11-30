#include <SDL.h>
#include <iostream>
#include <emscripten/html5.h>
#include <emscripten.h>

#include "EngineSource.hpp"
#include "game/GameMaster.hpp"

// Here's our game manager!
GameMaster* gm = nullptr;

blackjack::Card* c = new blackjack::Card(3, blackjack::BJ_Suit::SPADE);

// gameloop defined below
EM_BOOL gameloop(double time, void* userdata) {
	(void)userdata;
	// <><><><><><><><>
	// Updates our time and grabs user input
	// <><><><><><><><>
	if (!bob.tick(time))
		return EM_FALSE;

	// All game logic goes here!
	gm->blackjack();

	// Game master updates states of game logic and object paths
	gm->update();

	/*
	std::ccerr << c->to_string() << std::endl;
	std::cerr << c->id << std::endl;
	std::cerr << "pointer: " << c << std::endl;
	*/

	// <><><><><><><><>
	// Finally we render
	// <><><><><><><><>
	bob.render();

	return EM_TRUE;
}


int main() {

	SDL_Init(SDL_INIT_VIDEO);

	// GameMaster -> Handles game logic
	gm = new GameMaster();
	gm->initialize();									// GameMaster

	// Now we just pray that the sheetManager piped the input properly. :o

	/************ FUCK AROUND ZONE**********/

	bob.add(c);
	gm->add_card_to_hand({2, blackjack::BJ_Suit::HEART});
	gm->add_card_to_hand({3, blackjack::BJ_Suit::SPADE});
	gm->add_card_to_hand({55, blackjack::BJ_Suit::SPECIAL});
	gm->set_hand_as_target();
	c->set_path(gengine::Vertex(300,300,0), gengine::GENG_Path::BALLOON, 0.1, true);

	/******** END OF FUCK AROUND ZONE *******/
	emscripten_request_animation_frame_loop(gameloop, nullptr);
	emscripten_exit_with_live_runtime();

	// He's just a lil guy
	return 0;
}

