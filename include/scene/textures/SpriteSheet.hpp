#pragma once
#include "../../generics/Object.hpp"
#include "textures/Frame.hpp"
#include "Constants.hpp"
#include <string>
#include <vector>
#include <unordered_map>

// SpriteMap handle updating all objects tied to a sprite with proper sprite inform ation.
// It uses the property "Window" to determine how animations render.

using std::string;

class SpriteSheet {
	// Animation sheet that contains all sprites
	SDL_Texture* texture = nullptr;
	// Shadow of the sheet.
	SDL_Texture* shadow = nullptr;

	// Maps the STATE to a selection of FRAMES (animation window)
	std::unordered_map<uint8_t, std::vector<Frame>> frames;
	// Keeps track of all the textures for destruction at the end.
	std::vector<SDL_Texture*> textures;
	// Gives the path to the .ktx2 file
	string path;
	// Tells us the rendering type of this sheet
	uint8_t renderType = RENDER_FLAT;
	// Lets us quickly determine if our sheet is static or not
	bool fixed = true;

public:
	// Constructor
	SpriteSheet() = default;
	SpriteSheet(SDL_Texture *newSheet, uint8_t newRenderType);
	SpriteSheet(SDL_Texture *newSheet,
		uint8_t newRenderType,
		std::unordered_map<uint8_t, std::vector<Frame>> newFrames);
	SpriteSheet(int width, int height, int numColumns, int sprites,
			const string &p, const uint8_t renderT, uint8_t bhv, int dur);

	// Destructor
	~SpriteSheet();

	// Setters & Getters
	// Sets our current animation (and allows us to generate a shadow
	void set_texture(SDL_Texture *newSheet);
	void set_texture(SDL_Texture *newSheet, uint8_t newRenderType);
	// Allows us to set a custom shadow
	void set_shadow(SDL_Texture* newShadow);
	// Gets the animation
	[[nodiscard]] SDL_Texture* get_texture() const;
	// Gets the shadow
	[[nodiscard]] SDL_Texture* get_shadow() const;
	// Gets the render type
	[[nodiscard]] uint8_t get_renderType() const;

	// Allows us to tick an object to the next frame if neceesary
	void tick_frame(FrameState*&);

	/* Frame Code */
	void add_frames(std::vector<std::pair<int, std::vector<Frame>>> newFrames);
	void add_frames(int status_id, std::vector<Frame> newFrames);
	void add_frame(int status_id, Frame newFrame);
	SDL_Rect* get_frame(int state, int frame_id);

	// Gets the path and removes it from memory
	string pop_path();
	string to_string();

};