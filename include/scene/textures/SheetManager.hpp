#pragma once

// TextureManager handles updating all the sprite maps and objects with proper sprite information. It also handles removing sprites from objects. 
#include "textures/SpriteSheet.hpp"
// Incredibly important to include all the Reigstered Textures from the asset-info folder
#include "textures/asset-info/HeaderRegistry.hpp"
#include "textures/asset-info/SpriteRegistry.hpp"

// TextureManager is a class
class SheetManager {
private:
	// Links SpriteSheet IDs to a given SpriteSheet
	std::unordered_map<int, SpriteSheet> spriteSheets;
	// Lets us delete all unused textures at the end
	std::vector<SDL_Texture*> textures;

public:
	// Constructor
	// NEEDS WORK TO LOAD BITMAPS
	SheetManager();
	~SheetManager();

	// Loads all the sprites from the textures/asset-info folder
	void initialize(SDL_Renderer* renderer);

	// Creating new sheets
	// Create a sheet with one frame that doesn't repeat
	void create_sheet(int sheet_id, SDL_Texture* texture, SDL_Rect r, uint8_t renderType);
	// Create a sheet with an animation of repeating frames.
	void create_sheet(int sheet_id, SDL_Texture* texture, std::vector<Frame>, uint8_t renderType);
	// Creates a sheet with a fully fledged spriteset
	void create_sheet(int sheet_id, SDL_Texture* texture, std::vector<std::pair<int, std::vector<Frame>>> frameData, uint8_t renderType);

	// Texture Editing/Getting
	SDL_Texture* get_sheet_texture(int sheet_id);
	SDL_Texture* get_sheet_texture(const Object* o);
	void set_sheet_texture(int sheet_id, SDL_Texture *texture);
	// Texture Shadow/Editing/Getting
	SDL_Texture* get_sheet_shadow(int sheet_id);
	SDL_Texture* get_sheet_shadow(const Object* o);
	void set_sheet_shadow(int sheet_id, SDL_Texture *texture);
	// Rendering types
	uint8_t get_sheet_renderType(int sheet_id);
	uint8_t get_sheet_renderType(Object* o);
	// Get framesm
	SDL_Rect* get_sheet_frame(int sheet_id, int state, int frame_num);
	SDL_Rect* get_sheet_frame(Object* o);

	// Lets us tick an object
	void tick_frames(std::vector<FrameState*>&);

	// For printing
	string to_string();


};