#pragma once
// My files
#include "Camera.hpp"
#include "background-utilities/Background.hpp"
#include "../utilities/types/EngineElement.hpp"
#include "../sprites/SheetManager.hpp"
#include "engine/particles/ParticleGroup.hpp"
#include "engine/gengine-globals/scene.hpp"

// Libraries
#include  <emscripten/html5.h>
#include <SDL.h>


namespace gengine {
	class RenderManager {
		// PUBLIC FUNCTIONS
	public:
		explicit RenderManager(Camera* cam);
		~RenderManager();

		// Must call during main
		void initialize();

		// Initialize the RenderManager
		// The render pipeline is specified above. That is what this function uses.
		void render(std::vector<EngineElement>& elmts);
		// Present the render
		void present();

		// Gets our renderer for testing purposes
		SDL_Renderer* get_renderer() { return renderer; }

		void set_sheet_manager( SheetManager* sm);

	private:
		// RENDER PIPELINE
		// 1. Draws the background for our scene
		void draw_background();
		// 2. Then we render objects (flats, shadows, verticals)
		// BIG STEP ><><><><><><><
		// Divides elements between particles, objects, and UI
		void render_elements(std::vector<EngineElement>& elmts);
		// BIG STEP ><><><><><><><
		// <<<>>> Render Shadows & Verticals
		void render_object(Object& o);
		void render_particles(ParticleGroup &pg);
		// HELPER: Draw rectangles
		// Calculates where to draw rectangles on the canvas
		[[nodiscard]] SDL_FRect rect_flat(Object &o) const;

		SDL_FRect rect_flat(SDL_FRect f) const;

		[[nodiscard]] SDL_FRect rect_shadow(Object& o);
		[[nodiscard]] SDL_FRect rect_shadow(SDL_FRect f);

		SDL_FRect rect_shadow_offset(SDL_FRect f);

		SDL_FRect rect_particle_shadow(SDL_FRect &shadow);

		// Creates a shadow if one is not preloaded
		SDL_Texture* create_shadow_texture(SDL_Texture* texture);

		// Creates textures to render to
		void set_render_texture();

		// Updates the canvas in case the user decides to resize the window
		void update_canvas_size();
	private:
		// Specifies what we need to access SDL window resources
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		SheetManager* sm = nullptr;
		Background* bg = nullptr;
		Camera* cam;
		Vertex camCenter;


		// Scene width and height! */
		int canvasWidth = glb::scene.width;
		int canvasHeight = glb::scene.height;
		float scale = 1.0; 		// How much the user has scaled things
		bool setup = false; // If we're setup properly

		// This is the texture we draw to before scaling up.
		SDL_Texture* canvasTex;
	};
}
