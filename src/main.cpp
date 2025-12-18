#include <SDL.h>
#include <iostream>
#include <emscripten/html5.h>
#include <emscripten.h>

#include "Card.hpp"
#include "engine/Engine.hpp"
#include "engine/scene/morphs/morph-types/Stretch.hpp"
#include "engine/scene/particles/particle-types/Rhombus.hpp"
#include "engine/scene/particles/particle-types/Sparkle.hpp"
#include "game/asset-info/sheets/Asset-Card-Selector.hpp"
#include "game/asset-info/sheets/Asset-Card-Stack.hpp"
#include "game/asset-info/sheets/Asset-Deck.hpp"
#include "game/asset-info/sheets/sys-font.hpp"


// gameloop defined below
EM_BOOL gameloop(double time, void* userdata) {
	geng::Engine& bob = *static_cast<geng::Engine*>(userdata);
	(void)userdata;
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

	geng::Engine bob;
	bob.init();
	bob.set_debug_mode(true);
	bob.set_debug_immediate_print(true);

	auto* l = new geng::Layer("m");
	l->init.texture("assets/full-texture.png");
	l->init.set_sys_font("assets/full-texture.png", sys_font);
	l->init.frame_table("assets/full-texture.png", asset_deck);
	l->init.frame_table("assets/full-texture.png", asset_card_selector);
	l->init.frame_table("assets/full-texture.png", asset_draw_deck);
	bob.compose_layer(l);

	Card* c = new Card(59, blackjack::BJ_Suit::SPECIAL);
	c->set_draggable();
	l->apply_morph(new gfx::Stretch(c, 9.0, 1.0));
	l->attach_particle(c, new gfx::Rhombus(c, 10.f, 1.f, -1, 0.02));
	l->input.add_mouse_acceptor(c);
	l->add_actor(c);
	c->t.pos = {30, 50, 0};

	//l->cell_bucket.add_cell(0);


	/******** END OF FUCK AROUND ZONE *******/
	emscripten_request_animation_frame_loop(gameloop, &bob);
	emscripten_exit_with_live_runtime();

	return 0;
}

