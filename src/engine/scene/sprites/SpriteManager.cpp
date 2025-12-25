#include "engine/scene/sprites/SpriteManager.hpp"

#include "../../../../include/engine/types/strings/fstring/fstring.hpp"
#include "engine/debug/logging/LogSource.hpp"

using namespace geng;

// Constructor
SpriteManager::SpriteManager(LayerContext& layer_context) noexcept : scene(layer_context) {
	glog::note << "SpriteManager for " << scene.get_name() << " formed." << glog::endlog;
}

// Add object, pointer (preferred method)
void SpriteManager::add_sprite(Sprite *a) noexcept {
	// First we add the sprite to our link between ids and index
	id_to_pos[a->id] = sprites.size();
	// If our flag is notempty
	if (a->is_tagged())
		glog::note << "Sprite Tagged." << glog::endlog;
	// Then we just push it to the back of the vector
	sprites.push_back(a);
}

void SpriteManager::add_sprites(gch::vector<Sprite *>& vec) noexcept {
	for (auto& a: vec)
		add_sprite(a);
}

// We have to remove an object by its id
void SpriteManager::dissolve(int id) {
	// First we grab sprite's index
	if (id_to_pos.find(id) == id_to_pos.end()) {
		glog::err.src("SpriteManager::dissolve")
			<< "Id " << id << " does not exist." << glog::endlog;
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
void SpriteManager::dissolve(gch::vector<Sprite *> vec) {
	for (auto& a: vec)
		dissolve(a->id);
}

void SpriteManager::update(){
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

void SpriteManager::update_frames() {
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
