#include <SDL.h>
#include <emscripten/html5.h>
#include <emscripten.h>
#include <iostream>

#include "testing/Card.hpp"
#include "../include/engine/core/Engine.hpp"
#include "engine/scene/banners/text/Text.hpp"
#include "engine/scene/banners/widgets/WidgetBox.hpp"
#include "engine/scene/morphs/morph-types/Stretch.hpp"
#include "engine/scene/particles/particle-types/Rhombus.hpp"
#include "engine/scene/particles/particle-types/Sparkle.hpp"
#include "game/asset-info/sheets/Asset-Card-Stack.hpp"
#include "game/asset-info/sheets/Asset-Deck.hpp"
#include "engine/core/defaults/sysfont.inl"
#include "testing/PlayerTest.hpp"
#include "testing/spritesheet.hpp"

#include <engine/geng.hpp>

#include "testing/camerarouter.hpp"


geng::Collider* test;
auto start_time = std::chrono::steady_clock::now();

// gameloop defined below
bool gameloop(double time, void* userdata) {
	// Grabs our engine from main
	geng::Engine& bob = *static_cast<geng::Engine*>(userdata);

	bob.tick(time);
	// <><><><><><><><>
	// Updates our time and grabs user input & runs events
	// <><><><><><><><>
	bob.update();

	// <><><><><><><><>
	// Finally we render
	// <><><><><><><><>
	bob.render();

	return EM_TRUE;
}


int main() {

	// creates an engine object
	geng::Engine bob;
	// initializes the engine
	bob.init();
	// sets the window resolution
	bob.set_resolution({360, 160});
	// sets debug mode to true
	bob.set_debug_mode(true);
	// testing out the world texture
	bob.camera.pos = {0,0};
	bob.camera.set_dimensions({2000, 160});

	// *******************
	// Platformer
	// *******************
	/// Creates a new layer to store the world in

	auto& plat = *bob.create_layer<geng::Layer>("plat");
	plat.init.frame_table("assets/test_sprite.png", testsprite);
	plat.init.tileset("assets/levels/world1/level1.png", {64, 64, 8});
	plat.load_world("assets/levels/world1/world1.lvl", 0);
	bob.set_active_layer("platformer");


	// new sprites
	test = new PlayerTest(plat.get_frame_table(0));
	plat.link_collider(test);
	plat.input.add_mouse_acceptor(test);
	test->set_draggable();
	plat.input.add_key_press_acceptor(test);
	geng::FrameTable& testtable = plat.get_frame_table(0);

	auto camerarouter = bob.create_router<CameraRouter>();

	auto game_loop = [&]() {
		bob.camera.pos.x = test->t.pos.x - 50;
		return true;
	};

	GENG_START_LOOP(bob, game_loop);

	return 0;
}

