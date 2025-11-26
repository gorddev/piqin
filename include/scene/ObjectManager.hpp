#pragma once
#include "../generics/Object.hpp"
#include "textures/SheetManager.hpp"
#include "utilities/IDStack.hpp"
#include <unordered_map>

class ObjectManager {
private:
	// Links id -> object
	std::unordered_map<int, Object*> objList;
	// Allows us to keep track of object IDs
	IDStack stack;
	// Gives the number of objects
	int numObjects;
	// Gives the next available ID.
	int nextID = 0;

public:
	// We MUST provide a SheetManager
	explicit ObjectManager();

	// Adds an object to the objectManager
	Object* add_object(Object);
	Object* add_object(Object*);

	void remove_object(const Object *o);

	// Removes an object from the objectManager
	void remove_object(int id);
	// Updates properties of all objects.
	std::vector<FrameState*> update_objects();

	// Allows us to return our object list
	std::unordered_map<int, Object*>& get_objList();

	std::string to_string();

};