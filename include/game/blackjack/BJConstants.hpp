#pragma once

/* Z-INDEXING */
// Z-indexing for blackjack
#define SELECTOR_Z_BASE 13.0f
#define HAND_Z_BASE 10.0f
#define BOARD_Z_BASE 9.0f
#define DRAW_Z_BASE 8.0f
#define DECK_Z_BASE 7.0f

// Default positions
// CardDraws
#define BJ_DEFAULT_PLAYER_POS gengine::Vertex(3.3*gengine::glb::scene.width/8, 5*gengine::glb::scene.height/8.0, DRAW_Z_BASE)
#define BJ_DEFAULT_OPPONENT_POS gengine::Vertex(3.3*gengine::glb::scene.width/8, gengine::glb::scene.height/8.0, DRAW_Z_BASE)
// Floater card
#define BJ_DEFAULT_FLOATER_POS gengine::Vertex(7*gengine::glb::scene.width/8.0f, 4*gengine::glb::scene.height/6.0f, HAND_Z_BASE)
// Deck & discard
#define BJ_DEFAULT_DECK_POS gengine::Vertex(7*gengine::glb::scene.width/8.0f, 2.3*gengine::glb::scene.height/8, DECK_Z_BASE)
#define BJ_DEFAULT_DISCARD_POS gengine::Vertex(gengine::glb::scene.width/8.0f, 1.2*gengine::glb::scene.height/3.0f, DECK_Z_BASE)
#define BJ_DEFAULT_SELECTOR_POS gengine::Vertex(gengine::glb::scene.width/2.0f, 1.8f*gengine::glb::scene.height/2.0f, SELECTOR_Z_BASE)

// Selection offset
#define BJ_SELECT_OFFSET gengine::Vertex(0, 5, 0)
#define BJ_SELECTOR_OFFSET gengine::Vertex(0, -33, 0)
// Selection pathing
#define BJ_SELECT_PATH gengine::GENG_Path::BALLOON

// Increment Card
#define CARD_INCREMENT_VALUE 3
// For denying action
#define BJ_SHAKE_DENY gengine::GENG_Shake::SIDE, 3.0, 300, 0.5
#define BJ_SHAKE_FLOATER gengine::GENG_Shake::FLOATY, 5.0, -1, 2.0, false
#define BJ_SHAKE_FLOATER_TO_DISCARD gengine::GENG_Shake::RANDOM, 3.0, 450, 2.0, true
// For particles
#define BJ_PARTICLE_FLOATER