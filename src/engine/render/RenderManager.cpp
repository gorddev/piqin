#include "engine/render/RenderManager.hpp"
#include <string>

#include "engine/objects/Object.hpp"

using namespace gengine;

RenderManager::RenderManager(Camera* cam) :
		cam(cam), bg(nullptr), window(), renderer() {
	// Create the texture we will end up rendering to.
	canvasTex = nullptr;
	// Initializes good stuff
	vertices.resize(10000);
	vertices.clear();
}

void RenderManager::initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Norton",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			glb::scene.width, glb::scene.height, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	bg = new Background(renderer);
	canvasTex = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		glb::scene.width, glb::scene.height);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
}

RenderManager::~RenderManager() {
	delete cam;
}

void RenderManager::set_render_texture() {
	SDL_SetRenderTarget(renderer, canvasTex);
	SDL_SetRenderDrawColor(renderer, 242, 32, 21, 255);
	SDL_RenderClear(renderer);
}

void RenderManager::render(std::vector<EngineElement>& elmts) {

	/* RENDER SETUP */
	// First we set our draw color in case nothing renders
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	// We set up the camera center.
	camCenter = cam->center();
	// Then we update our canvas in case the user changed the window size
	update_canvas_size();
	// We flush the page
	SDL_RenderClear(renderer);
	/* RENDERING SHIT */

	vertices.clear();
	// Now we set our renderTexutre (glb::scene.width x glb::scene.height)
	// This will be scaled up after we draw the background & objects
	set_render_texture();
	// Draw the background
	draw_background();

	// ><><><><><>< BIG PHASE ><><><><><><
	render_elements(elmts);
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
	SDL_Rect fr = {0, 0, static_cast<int>(glb::scene.width * scale), static_cast<int>(glb::scene.height * scale)};
	// These next two lines center the canvas.
	fr.x = static_cast<int>((canvasWidth-(glb::scene.width*scale))/2.0);
	fr.y = static_cast<int>((canvasHeight-(glb::scene.height*scale))/2.0);

	// Now we scale up our whole canvas
	SDL_RenderCopy(
		renderer,
		canvasTex,
		nullptr,
		&fr);
	// Finally we present our hard work
	SDL_RenderPresent(renderer);
}

void RenderManager::set_texture_atlas(SDL_Texture *t) {
	atlas = t;
}

void RenderManager::draw_background() {
	bg->update(glb::scene.dt);
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
// Elements come presorted from the Engine
// <><><><><><><><><><><>><>
// Then, we separate each element into it's proper type and render it.
void RenderManager::render_elements(std::vector<EngineElement>& elmts) {
	for (auto& e: elmts) {
		// If our mystery item is an object
		if (e.type == GENG_Type::OBJECT) {
			// Turn our void pointer into an object!
			auto& o = *reinterpret_cast<Object*>(e.target);
			if (!o.hidden) {
				o.fs.frame->append_vertices(vertices, o.t);
				add_shadow(6);
			}
		}

		else if (e.type == GENG_Type::PARTICLE) {
			auto* pg = reinterpret_cast<ParticleGroup*>(e.target);
			add_shadow(pg->to_vertex(vertices));
		}
		else if (e.type == GENG_Type::UI) {
			// Unimplemented
		}

	}
	SDL_RenderGeometry(
		renderer,
		atlas,
		vertices.data(),
		vertices.size(),
		nullptr,
		0);
}

std::string recttostring(SDL_Rect rect) {
	std::string ret = "rect: {" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ", " + std::to_string(rect.w) + ", " + std::to_string(rect.h) + "}";
	return ret;
}

std::string recttostring(SDL_FRect rect) {
	std::string ret = "rect: {" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ", " + std::to_string(rect.w) + ", " + std::to_string(rect.h) + "}";
	return ret;
}

void RenderManager::render_object(Object& o) {
	if (!o.hidden) {
		o.fs.frame->append_vertices(vertices, o.t);
	}
}

static SDL_FRect add_rect(SDL_FRect r1, SDL_FRect r2) {
	return {r1.x + r2.x, r1.y + r2.y, r1.w + r2.w, r1.h + r2.h};
}

void RenderManager::render_particles(ParticleGroup& pg) {

}

/* ............... */
/* UPDATE CANVAS   */
/* ............... */
// Updates our canvas size if the user resizes the window
void RenderManager::update_canvas_size(){
	double wid, hei;
	// Gets the size of the window
	emscripten_get_element_css_size("canvas", &wid, &hei);

	int w = static_cast<int>(round(wid));
	int h = static_cast<int>(round(hei));
	// If our window changes we update shit
	if (w != canvasWidth || h != canvasHeight) {
		// Convert our w and h to ints to they go into width and height
		canvasWidth = w;
		canvasHeight = h;
		// Updates the window size
		SDL_SetWindowSize(window, canvasWidth, canvasHeight);
		// Tells our renderer to update its logic size.
		SDL_RenderSetScale(renderer, 1.0f, 1.0f);
		SDL_RenderSetLogicalSize(renderer, 0, 0);

		// Set our new camera width and height
		cam->set_width(canvasWidth);
		cam->set_height(canvasHeight);

		// Updates our scale accordingly
		scale = ((1.0 * canvasHeight) / canvasWidth < 0.75) ? 1.0*canvasHeight/glb::scene.height : 1.0*canvasWidth/glb::scene.width;
		if (scale < 1.0) scale = 1.0;
	}
}

void RenderManager::add_shadow(int numVertices) {
	// First gets our size before we update with new vertices
	int oldsize = static_cast<int>(vertices.size());
	// Resize our vector accordingly.
	vertices.resize(oldsize + numVertices);
	// Now we copy the old memory to the new positions
	// First we get the location of the new data
	SDL_Vertex* loc = vertices.data() + oldsize;
	// And the size of each SDL_Vertex is 24, so we have 24*numVertices bytes to copy
	// Then we memcpy them cause it's fast
	std::memcpy(loc, loc - numVertices, numVertices*sizeof(SDL_Vertex));
	// Finally we update each vertex location
	for (int i = oldsize - numVertices; i < oldsize; i++) {
		// Store vertex in reference for easy access
		SDL_Vertex& vert = vertices[i];
		// Update its position
		vert.position = {
			vert.position.x + 0.02f*(vert.position.x - (glb::scene.width*0.5f)),
			vert.position.y + (0.02f*(vert.position.y - glb::scene.height*0.25f))
		};
		// Update it's color
		vert.color = {0, 0, 0, 50};
	}
}
