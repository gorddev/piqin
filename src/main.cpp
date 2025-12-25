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
#include "../include/engine/defaults/sysfont.inl"
#include "testing/PlayerTest.hpp"
#include "testing/spritesheet.hpp"

#include <engine/geng.hpp>


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




	std::cerr << "engine initialized\n";
	glog::dev.src("main.cpp()") << "testing logging" << glog::endlog;

	auto* cardGame = bob.create_layer<geng::Layer>("card game");
	glog::dev.src("main.cpp()") << "created layer" << glog::endlog;
	cardGame->init.frame_table("assets/full-texture.png", asset_deck);

	std::cerr << "created layer\n";

	// Testing to see if card stretching works!
	Card* c = new Card(cardGame->get_frame_table(0),  52, blackjack::BJ_Suit::SPECIAL);
	cardGame->add_sprite(c);
	c->set_draggable();
	cardGame->apply_morph(new gfx::Stretch(c, 2, 1, 1.0));
	cardGame->attach_particle(c, new gfx::Rhombus(c, 10.f, 1.f, -1, 20, {200,255,255,255}));
	//cardGame->attach_particle(c2, new gfx::Rhombus(c, 10.f, 1.f, -1, 20, {255,200,255,255}));
	cardGame->input.add_mouse_acceptor(c);
	cardGame->scene._z_index();

	/// Testing to see if banners work!
	auto* testbanner = new geng::Banner({30,30}, 160, 40);
	testbanner->z_index += 0.3;
	testbanner->add_widget(new geng::WidgetBox(-100, -100, {0, 0, 0, 150}));
	geng::Text<150>* text = new geng::Text<150>("guys [c:red] look i can type something out[c:orange].\n so many [c:blue] colors. [c:yellow] it's great!", cardGame->get_font(0));
	testbanner->add_widget(text);
	cardGame->add_banner(testbanner);
	cardGame->input.add_mouse_acceptor(testbanner);
	testbanner->set_draggable();

	/// Testing to see if particles work
	auto* pg = new gfx::Sparkle({100,100}, 5, 1.f, -1, 20, {120, 220, 80, 255});
	//cardGame->instantiate_particle(pg);
	cardGame->input.add_mouse_acceptor(pg);
	pg->set_draggable();

	// testing out the world texture
	bob.camera.pos = {0,0};
	bob.camera.set_dimensions({360, 160});

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

	//plat.cell_bucket.add_cell(0, {0.5,0.5});

	glog::note << "Excited entering main!\n";

	auto game_loop = [&]() {
		return true;
	};

	GENG_START_LOOP(bob, game_loop);

	/******** END OF FUCK AROUND ZONE *******/
	emscripten_request_animation_frame_loop(gameloop, &bob);
	emscripten_exit_with_live_runtime();



	return 0;
}

