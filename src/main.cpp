#include <SDL.h>

#define GAN_DEBUG

#include <engine/gan.hpp>
#include <engine/gan_plugins.hpp>

#include "engine/scene/morphs/morph-types/Stretch.hpp"
#include "engine/scene/particles/particle-types/RhombusRotateSpiral.hpp"
#include "game/initialization/card_game_initialization.hpp"
#include "game/initialization/engine_initialization.hpp"
#include "game/initialization/initialization_defaults.hpp"
#include "game/initialization/initialization_data/small_card_frame_table.hpp"
#include "testing/camerarouter.hpp"
#include "testing/Card.hpp"

void init_engine(gan::Engine& bob, bool debug_enabled);

int main() {
	// creates an engine object
	gan::Engine bob;
	// initializes it
	init_engine(bob, true);
	// sets the window resolution
	bob.set_resolution({init::default_res_w, init::default_res_h});

	auto& cards = *bob.create_layer<gan::Layer>("cards");
	cards.init.frame_table("../assets/cards/smallcard.png", init::small_card_table);
	init::init_card_layer(cards);

	/*
	gch::vector<gan::Sprite*> cardlist;
	cardlist.reserve(56);
	gch::vector<gan::ParticleGroup*> pglist;
	pglist.reserve(54);



	for (int i = 0; i < 13; i++) {
		for (int k = 0; k < 4; k++) {
			blackjack::BJ_Suit suit;
			if (k == 0)
				suit = blackjack::BJ_Suit::HEART;
			else if (k == 1)
				suit = blackjack::BJ_Suit::CLUB;
			else if (k == 2)
				suit = blackjack::BJ_Suit::DIAMOND;
			else
				suit = blackjack::BJ_Suit::SPADE;
			auto mycard = new Card(cards.get_frame_table(init::cards_ft_id), 1 + i, suit);
			mycard->t.pos = {i*80.f, 300+80 * (1.f+k)};
			cards.add_sprite(mycard);
			cardlist.push_back(mycard);
			cards.input.add_mouse_acceptor(mycard);
			cards.apply_morph(new gfx::Stretch(mycard, 0.6, 0.2, 0.4));

			//auto myparticle = new gfx::RhombusRotateSpiral(mycard, 26, 5, -1, 20, {230, 230, 255, 255});
			//pglist.push_back(myparticle);
			//cards.attach_particle(mycard, myparticle);
		}
	}*/

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
	bob.create_font("../assets/fonts/ka1.ttf", 1, 10);
	bob.create_font("../assets/fonts/RetroByte.ttf", 1, 16);
	// allows us to move camera with arrow keys
	bob.create_router<CameraRouter>();
}

