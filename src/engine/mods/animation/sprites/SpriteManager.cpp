#include "engine/scene/sprites/SpriteManager.hpp"

#include "../../../../include/engine/types/strings/fstring/fstring.hpp"
#include "../../../../include/engine/debug/geng_debug.hpp"

using namespace gan;

// Constructor
SpriteManager::SpriteManager() noexcept {}

// Add object, pointer (preferred method)
void SpriteManager::add_sprite(Sprite *a) noexcept {
	//id_to_pos[a->id] = sprites.size();
	// If our flag is notempty
	if (a->is_tagged())
		glog::note << "Sprite Tagged." << glog::endlog;
	// Then we just push it to the back of the vector
	sprites.push_back(a);
}

void SpriteManager::add_sprites(std::vector<Sprite *>& vec) noexcept {
	for (auto& a: vec)
		add_sprite(a);
}

// We take in an object and convert it to an id.
void SpriteManager::dissolve(const Sprite* a) {
	for (auto& s: sprites) {
		if (s == a) {
			std::swap(s, sprites.back());
			sprites.pop_back();
			break;
		}
	}
}

// This grabs ids from all sprites and removes them.
void SpriteManager::dissolve(std::vector<Sprite *> vec) {
	for (auto& a: vec)
		dissolve(a);
}

void SpriteManager::update(LayerCore& scene){
	for (auto& a: sprites){
		Sprite& sprite = *a;
		// Print out a flag for testing purposes.
		if (sprite.is_tagged())
			glog::note.src("SpriteManager::Update")
				<< ("Sprite Tagged");
		// Update our object's update functions
		sprite.update(scene.state);
	}
}

void SpriteManager::update_frames(LayerCore& scene) {
	for (auto& a: sprites) {
		a->update_frame(scene.state);
	}
}

auto SpriteManager::begin() {
	return sprites.begin();
}

auto SpriteManager::end() {
	return sprites.end();
}

fstring<40> SpriteManager::to_string() {
	fstring<40> ret;
	ret << "Printing SpriteManager: "
		<< "\n::Num sprites: " << static_cast<int>(sprites.size());
	ret << "\n";
	return ret;
}
