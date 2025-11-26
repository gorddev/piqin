#pragma once
// My files
#include "Engine.hpp"
#include "render/Render_Rect.hpp"
#include "render/TextureRenderInfo.hpp"

// Libraries
#include  <emscripten/html5.h>
#include <SDL.h>

#include "../background-utilities/Background.hpp"

class RenderManager {
private:
	// RENDER PIPELINE
	// 1. Draws the background for our scene
	void draw_background();
	// 2. Then we render objects (flats, shadows, verticals)
	// BIG STEP ><><><><><><><
	void render_objects();
	// BIG STEP ><><><><><><><
	// <<<>>> Render Flats
	void render_flats(std::unordered_map<int, std::vector<TextureRenderInfo*>>&) const;
	// <<<>>> Render Shadows & Verticals
	void render_verticals(std::vector<TextureRenderInfo>);
	// HELPER: Draw rectangles
	// Calculates where to draw rectangles on the canvas
	[[nodiscard]] SDL_FRect rect_flat(Render_Rect) const;
	[[nodiscard]] SDL_FRect rect_shadow(Render_Rect, double z) const;
	// Creates a shadow if one is not preloaded
	SDL_Texture* create_shadow_texture(SDL_Texture* texture);

	// Creates textures to render to
	void set_render_texture();

	// Updates the canvas in case the user decides to resize the window
	void update_canvas_size();

	// PUBLIC FUNCTIONS
public:
	RenderManager();
	~RenderManager();

	// Initialize the RenderManager
	void initialize();

	// The render pipeline is specified above. That is what this function uses.  
	void render();
	// Present the render
	void present();

	// Returns width
	[[nodiscard]] int w() const;
	// Returns height
	[[nodiscard]] int h() const;

	// Gets our renderer for testing purposes
	SDL_Renderer* get_renderer() { return renderer; }

private:
	// Specifies what we need to access SDL window resources
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Vertex camCenter;

	// Owns the background
	Background* bg;

	// Scene width and height! */
	int canvasWidth;
	int canvasHeight;
	double scale; 		// How much the user has scaled things

	// This is the texture we draw to before scaling up.
	SDL_Texture* canvasTex;

};
