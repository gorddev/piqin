#include <SDL.h>

#include <engine/gan.hpp>
#include <engine/gan_plugins.hpp>

#include "engine/mods/animation/Animator.hpp"
#include "engine/mods/banners/BannerModule.hpp"
#include "engine/mods/plugins/routes/RouteModule.hpp"

#include "game/asset-info/card_sprites.hpp"
#include "game/cards/single-cards/Card.hpp"
#include "game/GameMaster.hpp"
#include "game/initialization/engine_initialization.hpp"
#include "game/initialization/initialization_defaults.hpp"

void init_engine(gan::Engine& bob, bool debug_enabled);
gan::Layer* init_card_layer(gan::Engine &bob);

int main() {
	// Creates the engine object
	gan::Engine bob;
	// Initialize the Engine
	init_engine(bob, true);

	auto& cards = *init_card_layer(bob);

	/*
	card::Card* mycard = new card::Card(cards.mod<gan::Anim>()->get_ft(0), 1, card::Suit::CLUB);
	cards.mod<gan::Anim>()->add_sprite(mycard);
	mycard->t.pos = {150, 100};
	mycard->set_draggable();
	cards.input.add_mouse_acceptor(mycard);
	*/
	/*
	gan::Banner box_test({100,100}, 100, 100);
	box_test.add_widget(new gan::WidgetBox(-100, -100, {gan::max_alpha, gan::max_alpha, gan::max_alpha, 40}));
	box_test.add_widget(new gan::WidgetBox(20, 20, {gan::max_alpha, gan::max_alpha, 50, 60}));
	box_test.add_widget(new gan::Text<40>("[s:2][c:red]heyo look [c:blue]at me", *bob.get_font(2), gan::Align::CENTER));
	auto border = gan::WidgetBorder(1);
	box_test.add_background(&border);
	border.set_color(gan::color_blue);
	cards.add_banner(&box_test);
	box_test.set_draggable();
	cards.input.add_mouse_acceptor(&box_test);

	gfx::HoverBanner hover_test({100, 100}, {100, 100}, 1, {240, 213, 230, 70}, {gan::max_alpha, gan::max_alpha, gan::max_alpha, 170});
	cards.add_banner(&hover_test);
	hover_test.set_draggable();
	cards.input.add_mouse_acceptor(&hover_test);

	*/


	gan::Banner text_banner({100, 100}, 50, 50);
	gan::ColorText my_text("Blackjack: Round 1", *bob.get_font(2), {255, 0, 0, 255}, gan::Align::CENTER);
	text_banner.add_widget(&my_text);
	text_banner.set_draggable();
	cards.mod<gan::Banners>()->add_banner(&text_banner);
	cards.input.add_mouse_acceptor(&text_banner);



	auto gameMaster = bob.create_router<card::GameMaster>(cards);

	auto game_loop = [&]() {


		return true;
	};
	GENG_START_LOOP(bob, game_loop);

	glog::warn << "exiting game" << glog::endlog;
	return 0;
}

void init_engine(gan::Engine &bob, bool debug_enabled) {
	// enables debug mode
	bob.init(debug_enabled);
	bob.set_debug_mode(debug_enabled);
	// creates our first font
	bob.create_font("../assets/fonts/ka1.ttf", 1, 10, SDL_ScaleModeNearest);
	bob.create_font("../assets/fonts/RetroByte.ttf", 1, 15, SDL_ScaleModeNearest);
	// sets the resolution of the internal engine window
	bob.set_resolution({init::default_card_res_w*4, init::default_card_res_h*4});
}

gan::Layer* init_card_layer(gan::Engine &bob) {
	// create the layer from the engine
	auto cards = bob.create_layer<gan::Layer>("cards");
	cards->mods.add_bundle(gan::std_mods);
	cards->mods.add_module<gfx::Routes>();
	auto* anim = cards->mod<gan::Animator>();

	// initialize our frame tables
	anim->gen_ft(
		"../assets/cards/smallcard.png",
		init::card_table
	);
	// frame table!
	anim->gen_ft(
		"../assets/cards/smallcard.png",
		init::card_highlight
	);

	// Set the dimensions of the card layer.
	cards->camera.set_dimensions({init::default_card_res_w, init::default_card_res_h});
	return cards;
}

