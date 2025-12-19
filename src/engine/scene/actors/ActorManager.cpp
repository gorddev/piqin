#include "engine/scene/actors/ActorManager.hpp"
#include <iostream>

using namespace geng;

// Constructor
ActorManager::ActorManager(LayerContext& layer_context) noexcept : scene(layer_context) {}

// Add object, pointer (preferred method)
void ActorManager::add_actor(Actor *a) noexcept {
	// First we add the actor to our link between ids and index
	id_to_pos[a->id] = actors.size();
	// If our flag is notempty
	if (a->is_tagged())
		std:: cerr << "Actor Tagged."
					<< "\nObject Info:\n"
					<< a->to_string() << std::endl;
	// Then we just push it to the back of the vector
	actors.push_back(a);
}

void ActorManager::add_actors(const std::vector<Actor *>& vec) noexcept {
	for (auto& a: vec)
		add_actor(a);
}

// We have to remove an object by its id
void ActorManager::dissolve(int id) {
	// First we grab actor's index
	if (id_to_pos.find(id) == id_to_pos.end()) {
		scene.log(2, "Id " + std::to_string(id) + " not found",
				  "ActorManager::dissolve(int id)");
	}
	int index = id_to_pos[id];
	int last = static_cast<int>(actors.size()) - 1;
	// Swap only if not already last
	if (index != last) {
		std::swap(actors[index], actors.back());
		id_to_pos[actors[index]->id] = index;
	}
	// Then we erase the id from the map.
	id_to_pos.erase(id);
	// And delete our actor
	delete actors.back();
	// And remove it from the back
	actors.pop_back();
}

// We take in an object and convert it to an id.
void ActorManager::dissolve(const Actor* a) {
	dissolve(a->id);
}

// This grabs ids from all actors and removes them.
void ActorManager::dissolve(std::vector<Actor *> vec) {
	for (auto& a: vec)
		dissolve(a->id);
}

void ActorManager::update(){
	// Get our dt
	double dt = scene.time.get_dt();
	for (auto& a: actors){
		Actor& actor = *a;
		// Print out a flag for testing purposes.
		if (actor.is_tagged())
			std::cerr << "Actor Tagged:"
						<< actor.to_string() << std::endl;
		// Update our object's update functions
		actor.update(scene.time);
		// Then we update our object's frame information
		actor.update_frame(scene.time);
	}
}

auto ActorManager::begin() {
	return actors.begin();
}

auto ActorManager::end() {
	return actors.end();
}

std::string ActorManager::to_string() {
	std::string ret;
	ret += "Printing ActorManager: ";
	ret += "\n::Num actors: " + std::to_string(actors.size());
	ret += "\n";
	return ret;
}
