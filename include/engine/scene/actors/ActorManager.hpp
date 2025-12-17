#pragma once
#include "Actor.hpp"
#include "engine/utilities/IDStack.hpp"
#include <unordered_map>
#include "../layers/LayerContext.hpp"

namespace geng {
	/** @brief Updates objects and tells the @code gengine::FrameManager@endcode when to update a @code gengine::Actor's@endcode AnimInfo objects.
	 * If using the engine, you should not touch this object.
	 */
	class ActorManager {
	private:
		// Links id -> object
		std::unordered_map<int, Actor*> actorMap;
		// Allows us to keep track of object IDs
		gutils::IDStack stack;
		// Gives the number of actors
		int numObjects;
		// Gives the next available ID.
		int nextID = 0;

		/// Holds the LayerContex object for dt stuff
		LayerContext& scene;
	public:
		// Constructor
		explicit ActorManager(LayerContext& layer_context) noexcept;

		// Adds an object to the objectManager
		Actor* add_actor(Actor) noexcept;
		Actor* add_actor(Actor*) noexcept;
		void add_actors(const std::vector<Actor*>& vec) noexcept;

		// Removes an object from the objectManager
		void dissolve(int id);
		void dissolve(const Actor *a);
		void dissolve(std::vector<Actor*> vec);

		// Updates properties of all actors.
		std::vector<AnimInfo*> update_objects();

		// Allows us to return our object list
		std::unordered_map<int, Actor*>& get_actor_list();

		std::string to_string();
	};
}
