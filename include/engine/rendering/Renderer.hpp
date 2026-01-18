#pragma once

// Libraries
#include <SDL.h>

#include "RenderBuffer.hpp"
#include "../core/EngineContext.hpp"
#include "engine/debug/Console.hpp"
#include "engine/layers/Layer.hpp"
#include "engine/scene/initializer/TextureRegister.hpp"
#include "shadows/ShadowBank.hpp"

namespace gan {
	/** @brief The RenderManager takes in EngineElements from the Engine, and renders them.
	 * @details To specify a background for the scene, make a subclass of the
	 * @code ganine::Background@endcode class and call the @code engine.set_background(Background* b)@endcode function.
	 * @warning You should not interact with the RenderManager directly. The engine will handle it for you.
	*/
	class Renderer {
	public:
		/// Makes the renderer with a core, a textureregister, and a camera.
		explicit Renderer(EngineContext& world, TextureRegister& texreg);
		/// Destructor
		~Renderer();

		/// Sets our render resolution to this size.
		void set_render_resolution(uint16_t width, uint16_t height);

		/// Must call during engine setup
		void _init();
		/// Renders each engine element according to its type.
		void render(gch::vector<Layer *> &layers, debug::Console *console
				            = nullptr);
		/// Present the render to the canvas
		void present();
		/// Initializes a texture register
		void prime_tex_register(TextureRegister& reg);

		/// Renders one given font
		int render_font(Font* font, hstring path);

	private:
		// RENDER PIPELINE
		// Renders one layer and all it's components according to how it wants to be layered.
		void render_layer(Layer *&lay);

		// This sets our render texture to the small 300 x 200 window so we get pixel-perfect scaling.
		void set_render_texture(SDL_Texture *texture);

		void update_texture_cache(Layer *l,
		                          debug::Console *console);

		// Updates the canvas in case the user decides to resize the window
		void update_canvas_size(bool force = false);

	private:
		// <><><> General Rendering <><><>
		/// Lets us quickswap between shadows
		ShadowBank shadows;
		/// Holds all the buffer for vertices we will ever need
		RenderBuffer buffer;
		// <><><> Texture Externals <><><>
		/// Holds the texture register
		TextureRegister& texreg;

		// <><><> Rendering Essentials <><><>
		/// Window is the SDL created window we draw to
		SDL_Window* window = nullptr;
		/// Renderer is the SDL created renderer we use to draw textures with. Tied to the window
		SDL_Renderer* renderer = nullptr;

		// <><><> Holds maps to each canvas texture
		/// Maps each layer to their canvas texture
		std::unordered_map<Layer*, Texture> layer_cache;

		/// Scene width and height!
		int canvasWidth = 0.0f;
		int canvasHeight = 0.0f;

		/// This is the texture we draw to before scaling up. It is the effective "resolution" of our window.
		SDL_Texture* canvasTex = nullptr;

		/// Holds engine core information necessary for rendering
		EngineContext& world;

		friend class Engine;
	};
}
