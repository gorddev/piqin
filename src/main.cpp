#include <SDL.h>
#include <iostream>
#include <emscripten/html5.h>
#include <emscripten.h>

#include "render/RenderManager/RenderManager.hpp"
#include "game/GameMaster.hpp"
#include "Constants.hpp"
#include "Engine.hpp"
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
	// OM updates objects according to states and paths
	std::vector<FrameState*> fss = engine::om->update_objects();
	// Send frame states to SpriteSheets to update
	engine::sm->tick_frames(fss);
	// Then we can finally render and present everything
	rend->render();
	rend->present();

	return EM_TRUE;
}


int main() {
	/* ENGINE SETUP */
	// Camera
	game::camera = new Camera(0,0,ZTOP, scene::width, scene::height);
	// RenderManager -> renders the scene.
	rend = new RenderManager();
	// SheetManager -> manages our SpriteSheets
	engine::sm = new SheetManager();
	// ObjectManager -> manages our Objects
	engine::om = new ObjectManager();
	// Manages input
	engine::input = new InputManager(nullptr);
	// GameMaster -> Handles game logic
	gm = new GameMaster();




	// Now we initialize each of our objects.
	rend->initialize();									// Renderer
	engine::sm->initialize(rend->get_renderer());		// SheetManager
	gm->initialize();									// GameMaster

	// Now we just pray that the sheetManager piped the input properly. :o

	/************ FUCK AROUND ZONE**********/
	gm->add_card_to_hand({1,'h'});
	gm->add_card_to_hand({4,'c'});
	gm->add_card_to_hand({5,'d'});
	gm->add_card_to_hand({6,'s'});
	gm->add_card_to_hand({7,'c'});
	gm->add_card_to_hand({8,'c'});
	gm->add_card_to_hand({9,'c'});
	gm->set_hand_as_target();



	/******** END OF FUCK AROUND ZONE *******/
	emscripten_request_animation_frame_loop(gameloop, nullptr);
	emscripten_exit_with_live_runtime();

	// He's just a lil guy
	return 0;
}

