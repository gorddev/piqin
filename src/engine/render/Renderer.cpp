#include "engine/rendering/Renderer.hpp"
#include <string>

using namespace geng;

Renderer::Renderer(EngineContext& world, Camera* cam) : world(world), cam(cam), buffer(shadows) {
	// Create the texture we will end up rendering to.
	canvasTex = nullptr;
	// Initializes good stuff
	buffer.resize(10000);
	buffer.clear();
}

void Renderer::set_render_resolution(const uint16_t width, const uint16_t height) {
	// Destroy our canvas tex if it doesn't already exist. 
	if (canvasTex != nullptr)
		SDL_DestroyTexture(canvasTex);
	world._set_window_size(width, height);
	if (renderer != nullptr) {
		canvasTex = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			world.get_width(), world.get_height());
	}
	update_canvas_size(true);
}

void Renderer::_init() {
	// Initialize SDL Video
	SDL_Init(SDL_INIT_VIDEO);
	// Create the window we will use
	window = SDL_CreateWindow("Norton",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			world.get_width(), world.get_height(), SDL_WINDOW_RESIZABLE);
	// Form the renderer to the window we'll use
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	// Set the canvas size we'll render to. 
	set_render_resolution(world.get_width(), world.get_height());
	// Tells us to use nearest neighbor scaling. 
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
}

Renderer::~Renderer() {
	delete cam;
}

void Renderer::set_render_texture() {
	SDL_SetRenderTarget(renderer, canvasTex);
	SDL_SetRenderDrawColor(renderer, 30, 90, 30, 255);
	SDL_RenderClear(renderer);
}

void Renderer::render(std::vector<Layer*>& layers) {

	/* RENDER SETUP */
	// First we set our draw color in case nothing renders
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	// Then we update our canvas in case the user changed the window size
	update_canvas_size();
	// We flush the page
	SDL_RenderClear(renderer);
	/* RENDERING SHIT */

	buffer.clear();
	// Now we set our renderTexutre (global::scene.width x global::scene.height)
	// This will be world.get_scale()d up after we draw the background & actors
	set_render_texture();

	// ><><><><><>< BIG PHASE ><><><><><><
	for (auto& l : layers) {
		render_layer(l);
	}
}


/*******************/
/* RENDER FUNCTIONS */
/*******************/

// <><><><><><><><><><><>><>
// Elements come presorted from the Engine
// <><><><><><><><><><><>><>
// Then, we separate each element into it's proper type and render it.
void Renderer::render_layer(Layer*& lay) {
	// Grabs the draw batches from the layer
	std::vector<DrawBatch> batches = lay->_render_self(buffer);

	// Now we render each batch.
	for (auto& batch : batches) {
		SDL_RenderGeometry(
		renderer,
		batch.tex,
		buffer.data() + batch.start_index,
		batch.num_vertices,
		nullptr,
		0);
	}
	buffer.clear();

	for (auto& cell: lay->cell_bucket) {
		SDL_Texture* tex = lay->_find_texture(cell.texture_id);
		cell.to_vertex(buffer, world);
		SDL_RenderGeometry(
			renderer,
			tex,
			buffer.data(),
			buffer.size(),
			nullptr,
			0
			);
		buffer.clear();
	}
}

void Renderer::present() {
	// Sert our render to the backbuffer
	SDL_SetRenderTarget(renderer, nullptr);
	// Then we establish the thing we're writing to.

	// Otherwise we world.get_scale() without pixel perfect scaling
	// This is the rectangle we draw to if we don't use pixel perfect.
	SDL_Rect fr;
	// These next lines
	fr.x = world.get_borderX();
	fr.y = world.get_borderY();
	fr.w = (world.get_width() * world.get_scale());
	fr.h = (world.get_height() * world.get_scale());

	// Now we world.get_scale() up our whole canvas
	SDL_RenderCopy(
		renderer,
		canvasTex,
		nullptr,
		&fr);
	// Finally we present our hard work
	SDL_RenderPresent(renderer);
}

void Renderer::prime_tex_register(TextureRegister &reg) {
	for (auto& [str, id] : reg.path_to_textureID) {
		if (reg.ID_to_texture.find(id) == reg.ID_to_texture.end()) {
			Texture tex = IMGDecoder::load_image_as_texture(renderer, str);
			world.log(0, "Loading image to texture: " + tex.info.filename, "Renderer::prime_tex_register");
			reg.ID_to_texture.emplace(id, std::move(tex));
		}
	}
}

/* ............... */
/* UPDATE CANVAS   */
/* ............... */
// Updates our canvas size if the user resizes the window
void Renderer::update_canvas_size(bool force) {
	double cssWidth, cssHeight;
	emscripten_get_element_css_size("canvas", &cssWidth, &cssHeight);

	if (force || cssWidth != canvasWidth || cssHeight != canvasHeight) {
		canvasWidth = static_cast<int>(round(cssWidth));
		canvasHeight = static_cast<int>(round(cssHeight));

		float scaleX = canvasWidth / static_cast<float>(world.get_width());
		float scaleY = canvasHeight / static_cast<float>(world.get_height());
		float scale = std::min(scaleX, scaleY);

		// Compute borders to center the low-res render target
		float borderX = (canvasWidth - world.get_width() * scale) / 2.0f;
		float borderY = (canvasHeight - world.get_height() * scale) / 2.0f;

		world._set_border_size(borderX, borderY);
		world.set_scale(scale);
	}
}