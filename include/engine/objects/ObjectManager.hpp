#pragma once
#include "Object.hpp"
#include "../utilities/types/IDStack.hpp"
#include <unordered_map>

namespace gengine {
	class ObjectManager {
	private:
		// Links id -> object
		std::unordered_map<int, Object*> objMap;
		// Allows us to keep track of object IDs
		utils::IDStack stack;
		// Gives the number of objects
		int numObjects;
		// Gives the next available ID.
		int nextID = 0;

	public:
		ObjectManager() noexcept;

		// Adds an object to the objectManager
		Object* add_object(Object) noexcept;
		Object* add_object(Object*) noexcept;
		void add_objects(const std::vector<Object*>& vec) noexcept;

		// Lets us update z-indexes
		void update_z(Object* o);
		// Lets us update with a vector
		void update_z(std::vector<Object*>& batch, uint8_t sortStyle);

		// Removes an object from the objectManager
		void dissolve(int id);
		void dissolve(const Object *o);
		void dissolve(std::vector<Object*> vec);

		// Updates properties of all objects.
		std::vector<FrameState*> update_objects();

		// Allows us to return our object list
		std::unordered_map<int, Object*>& get_objList();
		std::vector<Object*> z_sorted_objList();

		std::string to_string();
	};
}