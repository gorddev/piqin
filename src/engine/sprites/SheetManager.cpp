#include "engine/sprites/SheetManager.hpp"
#include "engine/utilities/BasisDecoder.hpp"

using namespace gengine;

SheetManager::SheetManager() {}

// Single Sprite
void SheetManager::create_sheet(int sheet_id, SDL_Texture* texture, SDL_Rect r) {
	if (spriteSheets.find(sheet_id) == spriteSheets.end()) {
		spriteSheets[sheet_id] = SpriteSheet(texture);
		spriteSheets[sheet_id].add_frame(0, {r, -1, GENG_Anim::STOP});
	}
	else
		printf("Err: Attempted to create sheet with pre-existing ID.\n");
}

// Single Animation
void SheetManager::create_sheet(int sheet_id, SDL_Texture *texture, std::vector<Frame> frames) {
	if (spriteSheets.find(sheet_id) == spriteSheets.end()) {
		spriteSheets[sheet_id] = SpriteSheet(texture);
		spriteSheets[sheet_id].add_frames(0, std::move(frames));
	}
	else
		printf("Err: Attempted to create sheet with pre-existing ID.\n");
}

// Full Sprite
void SheetManager::create_sheet(int sheet_id, SDL_Texture *texture, std::vector<std::pair<int, std::vector<Frame>>> frameData) {
	if (spriteSheets.find(sheet_id) == spriteSheets.end()) {
		spriteSheets[sheet_id] = SpriteSheet(texture);
		spriteSheets[sheet_id].add_frames(std::move(frameData));
	}
	else
		printf("Err: Attempted to create sheet with pre-existing ID.\n");
}

void SheetManager::set_sheet_texture(int sheet_id, SDL_Texture *texture) {
	textures.emplace_back(spriteSheets[sheet_id].get_texture());
	spriteSheets[sheet_id].set_texture(texture);
}

SDL_Texture* SheetManager::get_sheet_texture(int sheet_id) {
	return spriteSheets[sheet_id].get_texture();
}

SDL_Texture * SheetManager::get_sheet_texture(const Object& o) {
	return spriteSheets[o.sheet_id()].get_texture();
}

SDL_Texture * SheetManager::get_sheet_shadow(int sheet_id) {
	return spriteSheets[sheet_id].get_shadow();
}

SDL_Texture * SheetManager::get_sheet_shadow(const Object& o) {
	return spriteSheets[o.sheet_id()].get_shadow();
}

void SheetManager::set_sheet_shadow(int sheet_id, SDL_Texture *texture) {
	textures.emplace_back(spriteSheets[sheet_id].get_shadow());
	spriteSheets[sheet_id].set_shadow(texture);
}

uint8_t SheetManager::get_sheet_renderType(
	int sheet_id) {
	return spriteSheets[sheet_id].get_renderType();
}

uint8_t SheetManager::get_sheet_renderType(Object& o) {
	return spriteSheets[o.sheet_id()].get_renderType();
}



SDL_Rect* SheetManager::get_sheet_frame(int sheet_id, int state, int frame_num) {
	return spriteSheets[sheet_id].get_frame(state, frame_num);
}

SDL_Rect* SheetManager::get_sheet_frame(const Object& o) {
	return spriteSheets[o.sheet_id()].get_frame(o.state(), o.frameNum());
}

void SheetManager::tick_frames(std::vector<FrameState*>& fss) {
	for (auto& fs: fss) {
		spriteSheets[fs->sheet_id].tick_frame(fs);
	}
}

std::string SheetManager::to_string() {
	std::string ret = "SheetManager::to_string():\n"
				"- Number sheets: " + std::to_string(spriteSheets.size());
	ret += "\n- Num Extra textures: " + std::to_string(textures.size());
	ret += "\nNow printing all spriteSheets:\n";
	for (auto& [i, sheet] : spriteSheets) {
		ret += "Sheet id: " + std::to_string(i) + "\n";
		ret += sheet.to_string() + "\n";
	}
	return ret;
}

SheetManager::~SheetManager() {
	for (auto & i : textures) {
		SDL_DestroyTexture(i);
	}
	for (auto & [i, ss] : spriteSheets) {
		SDL_DestroyTexture(ss.get_texture());
		SDL_DestroyTexture(ss.get_shadow());
	}
}


void SheetManager::initialize(SDL_Renderer* renderer) {
	// Goes through our startup registry
	for (auto& [i, sheet] : getRegistry()) {
		// If this key doesn't already exist
		if (spriteSheets.find(i) == spriteSheets.end()) {
			// Creat our sheet
			spriteSheets[i] = sheet;
			// If texture doesn't exist, we make it.
			if (sheet.get_texture() == nullptr)
				// Decodes the texture!
				spriteSheets[i].set_texture(
					BasisDecoder::load_ktx2_as_texture(renderer, spriteSheets[i].pop_path()));
		}
		// else print error
		else
			printf("SheetManager: ERR Key %d already exists.\n", i);
	}
}



