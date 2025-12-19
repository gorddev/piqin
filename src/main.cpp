#include <SDL.h>
#include <iostream>
#include <emscripten/html5.h>
#include <emscripten.h>

#include "Card.hpp"
#include "engine/Engine.hpp"
#include "engine/scene/banners/text/Text.hpp"
#include "engine/scene/banners/widgets/WidgetBox.hpp"
#include "engine/scene/morphs/morph-types/Stretch.hpp"
#include "engine/scene/particles/particle-types/Rhombus.hpp"
#include "engine/scene/particles/particle-types/Sparkle.hpp"
#include "game/asset-info/sheets/Asset-Card-Selector.hpp"
#include "game/asset-info/sheets/Asset-Card-Stack.hpp"
#include "game/asset-info/sheets/Asset-Deck.hpp"
#include "game/asset-info/sheets/sys-font.hpp"


// gameloop defined below
EM_BOOL gameloop(double time, void* userdata) {
	// Grabs our engine from main
	geng::Engine& bob = *static_cast<geng::Engine*>(userdata);

	// <><><><><><><><>
	// Updates our time and grabs user input & runs events
	// <><><><><><><><>
	bob.tick(time);

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
	bob.set_resolution({400, 260});
	// sets debug mode to true
	bob.set_debug_mode(true);
	// immediately prints all debug notifications
	bob.set_debug_immediate_print(true);


	auto* cardGame = new geng::Layer("cardgame");
	cardGame->init.set_sys_font("assets/full-texture.png", sys_font);
	cardGame->init.frame_table("assets/full-texture.png", asset_deck);
	cardGame->init.frame_table("assets/full-texture.png", asset_card_selector);
	cardGame->init.frame_table("assets/full-texture.png", asset_draw_deck);
	bob.compose_layer(cardGame);

	// Testing to see if card stretching works!
	Card* c = new Card(cardGame->get_frame_table(0),  59, blackjack::BJ_Suit::SPECIAL);
	Card* c2 = new Card(cardGame->get_frame_table(0),55, blackjack::BJ_Suit::SPECIAL);
	cardGame->add_actor(c);
	cardGame->add_actor(c2);
	c->set_draggable();
	c2->set_draggable();
	c2->t.pos.z -= 0.3;
	cardGame->apply_morph(new gfx::Stretch(c, 1.0, 1.0, 2.0));
	cardGame->apply_morph(new gfx::Stretch(c2, 1.0, 1.0, 2.0));
	//cardGame->attach_particle(c, new gfx::Rhombus(c, 10.f, 1.f, -1, 20, {200,255,255,255}));
	//cardGame->attach_particle(c2, new gfx::Rhombus(c, 10.f, 1.f, -1, 20, {255,200,255,255}));
	cardGame->input.add_mouse_acceptor(c);
	cardGame->input.add_mouse_acceptor(c2);
	cardGame->scene._z_index();

	/// Testing to see if banners work!
	auto* testbanner = new geng::Banner({100,100,0}, 160, 40);
	testbanner->t.pos.z += 0.3f;
	testbanner->add_widget(new geng::WidgetBox(-100, -100, {0, 0, 0, 150}));
	geng::Text* text = new geng::Text("guys ^c:red^ look i can type something out^c:orange^.\n newlines maybe", cardGame->get_font(0));
	testbanner->add_widget(text);
	cardGame->add_banner(testbanner);
	cardGame->input.add_mouse_acceptor(testbanner);
	testbanner->set_draggable();
	std::cerr << "testbanner texid: " <<testbanner->get_texture_id() << std::endl;

	/// Testing to see if particles work
	auto* pg = new gfx::Sparkle({100,100,30}, 5, 1.f, -1, 20, {120, 220, 80, 255});
	//cardGame->instantiate_particle(pg);
	cardGame->input.add_mouse_acceptor(pg);
	pg->set_draggable();

	c->t.pos = {30, 50, 0};





	/******** END OF FUCK AROUND ZONE *******/
	emscripten_request_animation_frame_loop(gameloop, &bob);
	emscripten_exit_with_live_runtime();

	return 0;
}

