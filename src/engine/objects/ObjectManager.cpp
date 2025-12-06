#include "engine/objects/ObjectManager.hpp"
#include <format>
#include <iostream>

using namespace gengine;

// Constructor
ObjectManager::ObjectManager() noexcept : numObjects(0) {}

// Add object, pointer (preferred method)
Object * ObjectManager::add_object(Object *o) noexcept {
	// We need to assign it the correct id
	if (o->id != -1 && objMap.find(o->id) != objMap.end())
		return nullptr;
	if (!o->flag.empty())
		std:: cerr << o->flag << std::endl;
	objMap[o->id] = o;
	return o;
}

// Adding an object (non-pointer)
Object* ObjectManager::add_object(Object obj) noexcept {
	auto* o = new Object(obj);
	return add_object(o);
}

void ObjectManager::add_objects(const std::vector<Object *>& vec) noexcept {
	for (auto& o: vec)
		add_object(o);
}

// We have to remove an object by its id
void ObjectManager::dissolve(int id) {
	// Exit if the object doesn't exist
	if (objMap.find(id) == objMap.end()) return;
	// Otherwise, we grab the id and push it onto the id stack
	stack.push(id);
	// Then, we can remove the object from our map
	delete objMap[id];
	objMap.erase(id);
}

// We take in an object and convert it to an id.
void ObjectManager::dissolve(const Object* o) {
	dissolve(o->id);
}

// This grabs ids from all objects and removes them.
void ObjectManager::dissolve(std::vector<Object *> vec) {
	for (auto& o: vec)
		dissolve(o->id);
}

std::vector<FrameState*> ObjectManager::update_objects(){
	std::vector<FrameState*> ret;
	for (auto& [id, o]: objMap){
		Object& obj = *o;
		// Print out a flag for testing purposes.
		if (!obj.flag.empty())
			std::cerr << "Flag {id: " << id << ", " << o->flag << "}" << std::endl;
		// Update our object's position
		obj.update_pos();
		// Ticks our object's animation frame
		// SheetManager -> Object's Sheet -> tick_object
		if (obj.update_anim()) {
			ret.push_back(&obj.fs);
		}
	}
	return ret;
}

std::unordered_map<int, Object*>& ObjectManager::get_objList() {
	return objMap;
}

std::string ObjectManager::to_string() {
	std::string ret;
	ret += "Printing ObjectManager: ";
	ret += "\n::Num objects: " + std::to_string(objMap.size());
	ret += "\n::Stack size: " + std::to_string(stack.num_elements());
	ret += "\n::Stack memory: " + std::to_string(stack.size());
	ret += "\n::Stack top: " + std::to_string(stack.top());
	ret += "\n::Next ID: " + std::to_string(nextID);
	ret += "\n";
	return ret;

}
