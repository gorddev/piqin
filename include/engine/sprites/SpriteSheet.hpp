#pragma once
#include "../objects/Object.hpp"
#include "Frame.hpp"
#include "../gengine-globals/EngineEnums.hpp"
#include <string>
#include <vector>
#include <unordered_map>

// SpriteMap handle updating all objects tied to a sprite with proper sprite inform ation.
// It uses the property "Window" to determine how animations render.

namespace gengine {
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
		std::string path;
		// Lets us quickly determine if our sheet is static or not
		bool fixed = true;

	public:
		// Constructor
		SpriteSheet() = default;
		SpriteSheet(SDL_Texture *newSheet);
		SpriteSheet(SDL_Texture *newSheet, std::unordered_map<uint8_t, std::vector<Frame>> newFrames);
		SpriteSheet(int width, int height, int numColumns, int sprites,
				const std::string &p, GENG_Anim bhv, int dur);

		// Destructor
		~SpriteSheet();

		// Setters & Getters
		// Sets our current animation (and allows us to generate a shadow
		void set_texture(SDL_Texture *newSheet);
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
		std::string pop_path();
		std::string to_string();
	};
}