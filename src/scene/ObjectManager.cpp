#include "../../include/scene/ObjectManager.hpp"
#include <format>
#include <iostream>
#include <regex>

// Constructor
ObjectManager::ObjectManager() : numObjects(0) {}

// Adding an object
Object* ObjectManager::add_object(Object obj) {
	// If our IDStack is empty we just get the next avaliable ID
	if (objList.find(obj.id) != objList.end())
		return nullptr;
	obj.id = 0;
	auto* o = new Object(obj);
	if (stack.is_empty())
		o->id = nextID++;
	// Otherwise we reuse an ID from the stack
	else
		o->id = stack.pop();
	// Add to the object list
	objList[o->id] = o;
	return o;
}

Object * ObjectManager::add_object(Object *o) {
	if (objList.find(o->id) != objList.end())
		return nullptr;
	if (stack.is_empty())
		o->id = nextID++;
	// Otherwise we reuse an ID from the stack
	else
		o->id = stack.pop();
	// Add to the object list
	objList[o->id] = o;
	return o;
}

;
void ObjectManager::add_objects(std::vector<Object *>& vec) {
	for (auto& o: vec) {
		add_object(o);
	}
}

// Removing an object
void ObjectManager::remove_object(const Object* o) {
	remove_object(o->id);
}

void ObjectManager::remove_object(int id) {
	// Exit if the object doesn't exist
	if (objList.find(id) == objList.end()) return;
	// Otherwise, we grab the id and push it onto the id stack
	stack.push(id);
	// Then, we can remove the object from our map
	delete objList[id];
	objList.erase(id);
}

std::vector<FrameState*> ObjectManager::update_objects(){
	std::vector<FrameState*> ret(numObjects);
	for (auto& [id, o]: objList){
		// If object is flagged for removal, remove it.
		if (o->remove) {
			remove_object(o->id);
			continue;
		}
		if (!o->flag.empty())
			std::cerr << "Flag {id: " << id << ", " << o->flag << "}" << std::endl;
		// Update our object's position
		o->update_pos(scene::dt);
		// Ticks our object's animation frame
		// SheetManager -> Object's Sheet -> tick_object
		ret.push_back(o->frame_state());
	}
	return ret;
}

std::unordered_map<int, Object*>& ObjectManager::get_objList() {
	return objList;
}

std::vector<Object *> ObjectManager::z_sorted_objList() {
	std::vector<Object*> ret;
	ret.reserve(objList.size());
	for (auto& [id, o]: objList)
		ret.emplace_back(o);
	std::sort(ret.begin(), ret.end(), [](Object* a, Object* b) {
		return a->get_z_index() < b->get_z_index();
	});
	return ret;
}

std::string ObjectManager::to_string() {
	string ret;
	ret += "Printing ObjectManager: ";
	ret += "\n::Num objects: " + std::to_string(objList.size());
	ret += "\n::Stack size: " + std::to_string(stack.num_elements());
	ret += "\n::Stack memory: " + std::to_string(stack.size());
	ret += "\n::Stack top: " + std::to_string(stack.top());
	ret += "\n::Next ID: " + std::to_string(nextID);
	ret += "\n";
	return ret;

}
