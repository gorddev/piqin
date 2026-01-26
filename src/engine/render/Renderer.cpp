#include <utility>

#include "engine/rendering/Renderer.hpp"


#include "engine/debug/Console.hpp"

using namespace gan;

Renderer::Renderer(EngineContext& world)
		: world(world), buffer(world.tex_reg, shadows) {
	// Create the texture we will end up rendering to.
	canvasTex = nullptr;
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
	// Tells us to use nearest neighbor scaling.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	// Create the window we will use
	window = SDL_CreateWindow("Norton",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			world.get_width(), world.get_height(), SDL_WINDOW_RESIZABLE);
	// Form the renderer to the window we'll use
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//SDL_GL_SetSwapInterval(1);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	// Set the canvas size we'll render to.
	set_render_resolution(world.get_width(), world.get_height());


	// Creates the sysfont texture
	world.tex_reg.instantiate_texture(IMGDecoder::create_sysfont_texture(renderer));
}

Renderer::~Renderer() {
	for (auto& [id, t] : world.tex_reg.id_to_tex) {
		SDL_DestroyTexture(t.texture);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Renderer::set_render_texture(SDL_Texture* texture) {
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 30);
	SDL_RenderClear(renderer);
}

void Renderer::update_texture_cache(Layer* l, debug::Console* console) {

	const Camera& cam = (l == nullptr) ? console->cam : l->camera;

	// If we don't currently have the texture
	if (!layer_cache.contains(l)) {
		// create the layer-specific cache
		auto temptex = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			cam.get_width(), cam.get_height());
		// set the texture blend mode
		SDL_SetTextureBlendMode(temptex, SDL_BLENDMODE_BLEND);
		layer_cache[l] = Texture({"__cache", static_cast<uint32_t>(cam.get_width()), static_cast<uint32_t>(cam.get_height())}, temptex);
	}
	// If our layer needs to be updated because the camera changed.
	else if (layer_cache[l].info.h != cam.get_height()
		|| layer_cache[l].info.w != cam.get_width()) {
		SDL_DestroyTexture(layer_cache[l].texture);
		layer_cache[l].texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			cam.get_width(), cam.get_height());
		SDL_SetTextureBlendMode(layer_cache[l].texture, SDL_BLENDMODE_BLEND);
	}
}

void Renderer::render(std::vector<Layer*>& layers, debug::Console* console) {

	/* RENDER SETUP */
	// First we set our draw color in case nothing renders
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, gan::max_alpha);
	// Then we update our canvas in case the user changed the window size
	update_canvas_size();
	// We flush the page
	SDL_RenderClear(renderer);

	// Now we check if any textures need rendering
	if (world.tex_reg.dirty) {
		for (auto& [str, id]: world.tex_reg.ready_textures) {
			world.tex_reg.load_texture(id, IMGDecoder::load_image_as_texture(renderer, str));
		}
		world.tex_reg.ready_textures.clear();
		world.tex_reg.dirty = false;
	}
	// Now we set our renderTexutre (global::scene.width x global::scene.height)
	set_render_texture(canvasTex);

	buffer.clear();

	// ><><><><><>< BIG PHASE ><><><><><><
	for (auto& layer : layers) {
		// Only render if the layer is visible.
		if (layer->scene.is_visible()) {
			// ********************************
			// <><><> Render preparation <><><>
			// Update our texture cache to make sure we have a scene to render to.
			update_texture_cache(layer, console);
			// set our render target to the layer's specific texture
			set_render_texture(layer_cache[layer].texture);
			// clear the renderer
			SDL_RenderClear(renderer);
			// prep the render buffer with camera information
			buffer.prep(layer->camera, {layer->camera.get_width(), layer->camera.get_height()});

			// ***********************
			// <><><> Rendering <><><>
			// render the specific layer
			render_layer(layer);
			// pop the final batch off the buffer
			buffer.pop_last_batch();
			// render everything to our small canvas
			for (auto& batch : buffer.batches) {
				SDL_RenderGeometry(
				renderer,
				world.tex_reg[batch.texture_id].texture,
				buffer.data() + batch.start_index,
				batch.num_vertices,
				nullptr,
				0);
			}

			// ************************
			// <><><> Pushing to canvas.
			// clear our buffer
			buffer.clear();
			// set our render target to be the main canvas
			SDL_SetRenderTarget(renderer, canvasTex);
			// copy it!
			SDL_RenderCopy(renderer, layer_cache[layer].texture, nullptr, nullptr);
		}
	}

	if (console != nullptr) {
		// Update the texture cache with console information
		update_texture_cache(nullptr, console);
		// prep the buffer
		buffer.prep(console->cam, {console->cam.get_width(), console->cam.get_height()});
		buffer.request_texture(0);
		// add all of our vertices to the buffer
		console->to_vertex(buffer);
		// pop our last batch
		buffer.pop_last_batch();
		// Sets the render texture to be our console's
		set_render_texture(layer_cache[nullptr].texture);
		// clear the render target
		SDL_RenderClear(renderer);
		// render to our texture
		for (auto& batch : buffer.batches) {
			SDL_RenderGeometry(
			renderer,
			world.tex_reg[batch.texture_id].texture,
			buffer.data() + batch.start_index,
			batch.num_vertices,
			nullptr,
			0);
		}
		// set our render target to be the main canvas
		SDL_SetRenderTarget(renderer, canvasTex);
		// copy it!
		SDL_RenderCopy(renderer, layer_cache[nullptr].texture, nullptr, nullptr);
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

	// <><><> First the layer renders itself
	lay->render_self(buffer);

	// <><><> Here's where we render the cell bucket.
	for (auto& cell: lay->cell_bucket) {
		buffer.request_texture(cell.texture_id);
		cell.to_vertex(buffer, world.get_width(), world.get_height() );
	}
}

void Renderer::present() {
	// Sert our render to the backbuffer
	SDL_SetRenderTarget(renderer, nullptr);
	// Then we establish the thing we're writing to.

	// Otherwise we engine_context.get_scale() without pixel perfect scaling
	// This is the rectangle we draw to if we don't use pixel perfect.
	SDL_Rect fr;
	// These next lines
	fr.x = world.get_borderX();
	fr.y = world.get_borderY();
	fr.w = (world.get_width() * world.get_scale());
	fr.h = (world.get_height() * world.get_scale());

	// Now we engine_context.get_scale() up our whole canvas
	SDL_RenderCopy(
		renderer,
		canvasTex,
		nullptr,
		&fr);
	// Finally we present our hard work
	SDL_RenderPresent(renderer);
}

void Renderer::prime_tex_register(TextureRegister &reg) {
	for (auto& [str, id] : reg.path_to_id) {
		if (reg.id_to_tex.find(id) == reg.id_to_tex.end()) {
			Texture tex = IMGDecoder::load_image_as_texture(renderer, str);
			reg.id_to_tex.emplace(id, std::move(tex));
		}
	}
}

int Renderer::render_font(Font* font, hstring path, SDL_ScaleMode render_mode) {
	Texture tex = IMGDecoder::load_font_as_texture(renderer, *font, std::move(path), render_mode);
	return world.tex_reg.instantiate_texture(tex);
}

/* ............... */
/* UPDATE CANVAS   */
/* ............... */
// Updates our canvas size if the user resizes the window
void Renderer::update_canvas_size(bool force) {
	int cssWidth, cssHeight;
	SDL_GL_GetDrawableSize(window, &cssWidth, &cssHeight);

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