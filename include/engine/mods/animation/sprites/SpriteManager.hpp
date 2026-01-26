#pragma once
#include "Sprite.hpp"
#include "engine/utilities/IDStack.hpp"
#include <unordered_map>
#include "../../layers/layer-subobjects/LayerCore.hpp"

namespace gan {
	/** @brief Updates objects and tells the @code ganine::FrameManager@endcode when to update a @code ganine::Sprite's@endcode AnimInfo objects.
	 * If using the engine, you should not touch this object.
	 */
	class SpriteManager {
	private:
		/// Vector of all of the sprites in the scene
		std::vector<Sprite*> sprites;
		/// Links each of the sprites IDs to their position in the vector.
		std::unordered_map<int, int> id_to_pos;

	public:
		// Constructor
		explicit SpriteManager() noexcept;

		// Adds an object to the objectManager
		void add_sprite(Sprite*) noexcept;
		void add_sprites(std::vector<Sprite *> &vec) noexcept;

		// Removes an object from the objectManager
		void dissolve(const Sprite *a);
		void dissolve(std::vector<Sprite*> vec);

		// Updates properties of all sprites.
		void update(LayerCore &scene);
		// Updates the sprites of each sprites
		void update_frames(LayerCore &scene);

		// Allows us to return our object list
		auto begin();
		auto end();

		fstring<40> to_string();
	};
}
