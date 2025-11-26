#include "../../../../include/scene/render/RenderManager/RenderManager.hpp"
#include <iostream>

string rect_to_string(SDL_Rect r) {
	return "{" + std::to_string(r.x)
			+ ", " + std::to_string(r.y)
			+ ", " + std::to_string(r.w)
			+ ", " + std::to_string(r.h) + "}";
}

void RenderManager::initialize(){
	// Initializes the video scene
	SDL_Init(SDL_INIT_VIDEO);
	// Creates the window through which we'll view the game
	window = SDL_CreateWindow("Norton",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		canvasWidth, canvasHeight,
		SDL_WINDOW_RESIZABLE);
	// Creates the renderer so we can draw to the window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	bg = new Background(renderer);
}

void RenderManager::render() {

	/* RENDER SETUP */
	// First we set our draw color in case nothing renders
	SDL_SetRenderDrawColor(renderer, 20, 25, 0, 0);
	// We set up the camera center.
	camCenter = game::camera->center();
	// Then we update our canvas in case the user changed the window size
	update_canvas_size();
	// We flush the page
	SDL_RenderClear(renderer);
	/* RENDERING SHIT */

	// Now we set our renderTexutre (scene::width x scene::height)
	// This will be scaled up after we draw the background & objects
	set_render_texture();
	// Draw the background
	draw_background();
	// ><><><><><>< BIG PHASE ><><><><><><
	render_objects();
	// ><><><><><>< BIG PHASE ><><><><><><
	/* Now we scale up our texture in accordance with the gamera */
}



void RenderManager::present() {
	// Sert our render to the backbuffer
	SDL_SetRenderTarget(renderer, nullptr);
	// Then we establish the thing we're writing to.

	// If we're pixel perfect, we only use integer scalings.
	if (PIXEL_PERFECT)
		scale = static_cast<int>(scale);
	// Otherwise we scale without pixel perfect scaling
	// This is the rectangle we draw to if we don't use pixel perfect.
	SDL_Rect fr = {0, 0, static_cast<int>(scene::width * scale), static_cast<int>(scene::height * scale)};
	// These next two lines center the canvas.
	fr.x = static_cast<int>((canvasWidth-(scene::width*scale))/2.0);
	fr.y = static_cast<int>((canvasHeight-(scene::height*scale))/2.0);

	// Now we scale up our whole canvas
	SDL_RenderCopy(
		renderer,
		canvasTex,
		nullptr,
		&fr);
	// Now we need to free the texture
	SDL_DestroyTexture(canvasTex);
	// Finally we present our hard work
	SDL_RenderPresent(renderer);
}

void RenderManager::set_render_texture() {
	canvasTex = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		scene::width, scene::height);

	SDL_SetRenderTarget(renderer, canvasTex);
	SDL_SetRenderDrawColor(renderer, 20, 80, 20, 255);
	SDL_RenderClear(renderer);
}

void RenderManager::draw_background() {
	bg->update(scene::dt);
	SDL_RenderCopy(
		renderer,
		bg->get_texture(),
		nullptr,
		nullptr);
}


/*******************/
/* RENDER FUNCTIONS */
/*******************/

// <><><><><><><><><><><>><>
// Assume they are all sorted in order of render priority already.
// <><><><><><><><><><><>><>

void RenderManager::render_objects() {

	// Two unordered maps: sheet_id -> vector of rectangles to render with that sheet it
	std::unordered_map<int, std::vector<TextureRenderInfo*>> flats;
	std::vector<TextureRenderInfo>verticals;
	// Iterate through each of our objects
	for (auto& o: engine::om->z_sorted_objList()){
		// If an object is hidden we don't display it.
		if (o->hidden)
			continue;
		// If our object is fixed, we don't need to redo calculations.
		if (o->fixed && o->tex_render_info != nullptr) {
			uint8_t renderType = engine::sm->get_sheet_renderType(o->sheet_id());
			if (renderType == RENDER_FLAT) {
				flats[o->sheet_id()].emplace_back(o->tex_render_info);
				continue;
			}
		}

		delete o->tex_render_info;

		// Then get our sprite rectangle
		SDL_Rect* sprite = engine::sm->get_sheet_frame(o->sheet_id(), o->state(), o->frameNum());

		// If flat, then we render it as a flat
		SDL_FRect canvas = rect_flat(
			{o->pos().x,
				o->pos().y,
				o->width() * o->scale(),
				static_cast<double>(o->height() * o->scale())});
		o->tex_render_info = new TextureRenderInfo(canvas, sprite, o->sheet_id());
		// Add to flats map
		if (engine::sm->get_sheet_renderType(o) == RENDER_VERTICAL)
			verticals.emplace_back(*o->tex_render_info);
		else
			flats[o->sheet_id()].emplace_back(o->tex_render_info);
		// If not flat, then we render vertically
		if (engine::sm->get_sheet_renderType(o) == RENDER_VERTICAL) {
			//// SHADOW
			if (o->shadow == false) {
				TextureRenderInfo shadow({-1,-1,0,0}, nullptr, -1);
				verticals.emplace_back(shadow);
			}

			// If the shadow texture doesn't exist, we make it.
			if (engine::sm->get_sheet_shadow(o) == nullptr)
				engine::sm->set_sheet_shadow(o->sheet_id(), create_shadow_texture(engine::sm->get_sheet_texture(o)));
			SDL_FRect shadow = rect_shadow(
				{o->pos().x,
					o->pos().y,
					o->width() * o->scale(),
					o->height() * o->scale()},
					o->pos().z);
			verticals.emplace_back(shadow, sprite, o->sheet_id());
		}
	}
	// Render our flat objects
	render_flats(flats);
	// Then render our verticals
	render_verticals(verticals);
}





