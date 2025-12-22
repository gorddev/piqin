#pragma once
#include "Sprite.hpp"
#include "engine/utilities/IDStack.hpp"
#include <unordered_map>
#include "engine/layers/LayerContext.hpp"

namespace geng {
	/** @brief Updates objects and tells the @code gengine::FrameManager@endcode when to update a @code gengine::Sprite's@endcode AnimInfo objects.
	 * If using the engine, you should not touch this object.
	 */
	class SpriteManager {
	private:
		/// Vector of all of the sprites in the scene
		std::vector<Sprite*> sprites;
		/// Links each of the sprite's IDs to their position in the vector.
		std::unordered_map<int, int> id_to_pos;
		/// Holds the LayerContex object for dt stuff
		LayerContext& scene;
	public:
		// Constructor
		explicit SpriteManager(LayerContext& layer_context) noexcept;

		// Adds an object to the objectManager
		void add_sprite(Sprite*) noexcept;
		void add_sprites(const std::vector<Sprite*>& vec) noexcept;

		// Removes an object from the objectManager
		void dissolve(int id);
		void dissolve(const Sprite *a);
		void dissolve(std::vector<Sprite*> vec);

		// Updates properties of all sprites.
		void update();
		// Updates the sprites of each sprite
		void update_sprites();

		// Allows us to return our object list
		auto begin();
		auto end();

		std::string to_string();
	};
}
