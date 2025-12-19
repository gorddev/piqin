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
		/// Vector of all of the actors in the scene
		std::vector<Actor*> actors;
		/// Links each of the actor's IDs to their position in the vector.
		std::unordered_map<int, int> id_to_pos;
		/// Holds the LayerContex object for dt stuff
		LayerContext& scene;
	public:
		// Constructor
		explicit ActorManager(LayerContext& layer_context) noexcept;

		// Adds an object to the objectManager
		void add_actor(Actor*) noexcept;
		void add_actors(const std::vector<Actor*>& vec) noexcept;

		// Removes an object from the objectManager
		void dissolve(int id);
		void dissolve(const Actor *a);
		void dissolve(std::vector<Actor*> vec);

		// Updates properties of all actors.
		void update();

		// Allows us to return our object list
		auto begin();
		auto end();

		std::string to_string();
	};
}
