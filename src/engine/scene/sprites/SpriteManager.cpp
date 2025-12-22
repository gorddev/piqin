#include "engine/scene/sprites/SpriteManager.hpp"
#include <iostream>

using namespace geng;

// Constructor
SpriteManager::SpriteManager(LayerContext& layer_context) noexcept : scene(layer_context) {}

// Add object, pointer (preferred method)
void SpriteManager::add_sprite(Sprite *a) noexcept {
	// First we add the sprite to our link between ids and index
	id_to_pos[a->id] = sprites.size();
	// If our flag is notempty
	if (a->is_tagged())
		std:: cerr << "Sprite Tagged."
					<< "\nObject Info:\n"
					<< a->to_string() << std::endl;
	// Then we just push it to the back of the vector
	sprites.push_back(a);
}

void SpriteManager::add_sprites(const std::vector<Sprite *>& vec) noexcept {
	for (auto& a: vec)
		add_sprite(a);
}

// We have to remove an object by its id
void SpriteManager::dissolve(int id) {
	// First we grab sprite's index
	if (id_to_pos.find(id) == id_to_pos.end()) {
		scene.log(2, "Id " + std::to_string(id) + " not found",
				  "SpriteManager::dissolve(int id)");
	}
	int index = id_to_pos[id];
	int last = static_cast<int>(sprites.size()) - 1;
	// Swap only if not already last
	if (index != last) {
		std::swap(sprites[index], sprites.back());
		id_to_pos[sprites[index]->id] = index;
	}
	// Then we erase the id from the map.
	id_to_pos.erase(id);
	// And delete our sprite
	delete sprites.back();
	// And remove it from the back
	sprites.pop_back();
}

// We take in an object and convert it to an id.
void SpriteManager::dissolve(const Sprite* a) {
	dissolve(a->id);
}

// This grabs ids from all sprites and removes them.
void SpriteManager::dissolve(std::vector<Sprite *> vec) {
	for (auto& a: vec)
		dissolve(a->id);
}

void SpriteManager::update(){
	for (auto& a: sprites){
		Sprite& sprite = *a;
		// Print out a flag for testing purposes.
		if (sprite.is_tagged())
			std::cerr << "Sprite Tagged:"
						<< sprite.to_string() << std::endl;
		// Update our object's update functions
		sprite.update(scene.state);
	}
}

void SpriteManager::update_sprites() {
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

std::string SpriteManager::to_string() {
	std::string ret;
	ret += "Printing SpriteManager: ";
	ret += "\n::Num sprites: " + std::to_string(sprites.size());
	ret += "\n";
	return ret;
}
