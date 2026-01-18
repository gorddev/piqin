#pragma once
#include "Sprite.hpp"
#include "engine/utilities/IDStack.hpp"
#include <unordered_map>
#include "engine/layers/LayerContext.hpp"

namespace gan {
	/** @brief Updates objects and tells the @code ganine::FrameManager@endcode when to update a @code ganine::Sprite's@endcode AnimInfo objects.
	 * If using the engine, you should not touch this object.
	 */
	class SpriteManager {
	private:
		/// Vector of all of the sprites in the scene
		gch::vector<Sprite*> sprites;
		/// Links each of the sprites's IDs to their position in the vector.
		std::unordered_map<int, int> id_to_pos;
		/// Holds the LayerContex object for dt stuff
		LayerContext& scene;
	public:
		// Constructor
		explicit SpriteManager(LayerContext& layer_context) noexcept;

		// Adds an object to the objectManager
		void add_sprite(Sprite*) noexcept;
		void add_sprites(gch::vector<Sprite *> &vec) noexcept;

		// Removes an object from the objectManager
		void dissolve(const Sprite *a);
		void dissolve(gch::vector<Sprite*> vec);

		// Updates properties of all sprites.
		void update();
		// Updates the sprites of each sprites
		void update_frames();

		// Allows us to return our object list
		auto begin();
		auto end();

		fstring<40> to_string();
	};
}
