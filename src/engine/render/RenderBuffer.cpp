#include "engine/rendering/RenderBuffer.hpp"

using namespace gan;

// ------------------- Constructor -------------------

RenderBuffer::RenderBuffer(TextureRegister& texreg, ShadowBank& shadows)
    : texreg(texreg), shadows(shadows), current_batch(0) {
    batches.reserve(40);
    buffer.reserve(30000);
}

void RenderBuffer::request_texture(int id) {
    if (id >= 0 && id != current_batch.texture_id && texreg.is_loaded(id)) {
        // Sets the number of vertices of the draw batch
        current_batch.num_vertices = size() - current_batch.start_index;
        // Pushes back the current batch.
        batches.push_back(current_batch);
        // Sets the starting index to the current size
        current_batch.start_index = size();
        // Updated the id
        current_batch.texture_id = id;
        // Refreshes the loaded flag.
        loaded = true;
    }
    else if (!texreg.is_loaded(id)) {
        // Sets the loaded flag to false and now everything becomes a white texture.
        loaded = false;
    }
    else
        loaded = true;
}

void RenderBuffer::prep_vertex(SDL_Vertex &vertex) {
    if (!loaded)
        vertex.tex_coord = white_point;
}

// ------------------- Public getters -------------------

SDL_FPoint RenderBuffer::get_white_point() {
    return white_point;
}

void RenderBuffer::begin_object() {
    start_pos = buffer.size();
}

void RenderBuffer::end_object() {
    if (!debug) {
        screenScale.x = std::roundf(screenScale.x);
        screenScale.y = std::roundf(screenScale.y);
        for (int i = start_pos; i < buffer.size(); ++i) {
            float x =
                (buffer[i].position.x - cam.pos.x + cam.offset.x)
                * screenScale.x;

            float y =
                (buffer[i].position.y - cam.pos.y + cam.offset.y)
                * screenScale.y;

            buffer[i].position.x = x;
            buffer[i].position.y = y;
        }
    }
    start_pos = buffer.size();
}

void RenderBuffer::push_back(SDL_Vertex vertex) {
    if (!debug)
        prep_vertex(vertex);
    buffer.push_back(vertex);
}

void RenderBuffer::push_back(SDL_FPoint& pos, SDL_Color& color, SDL_FPoint& tex_coord) {
    push_back({pos, color, tex_coord});
}

void RenderBuffer::push_back(std::vector<SDL_Vertex>& vertices) {
    for (auto& v : vertices)
        prep_vertex(v);
    buffer.insert(buffer.end(), vertices.begin(), vertices.end());
}

void RenderBuffer::push_back(SDL_FPoint pos, SDL_Color color) {
    push_back({ pos, color, white_point });
}

void RenderBuffer::push_back(std::vector<SDL_FPoint>& pos, SDL_Color color) {
    for (auto& i : pos) {
        push_back(i, color);
    }
}

// ------------------- Shadow functions -------------------

ShadowBank& RenderBuffer::get_shadow_bank() {
    return shadows;
}


void RenderBuffer::push_shadow(int numShadows) {
    shadows.apply_shadow(buffer, numShadows);
}

void RenderBuffer::push_shadow() {
    shadows.apply_shadow(buffer, 3);
}


void RenderBuffer::instanced_push_shadow(std::vector<SDL_Vertex>& vertices, int numShadows) {
    shadows.apply_shadow(vertices, numShadows);
}

// ------------------- Renderer interface -------------------

int RenderBuffer::size() const {
    return static_cast<int>(buffer.size());
}

void RenderBuffer::prep(Camera& camera, dim2 canvasDim) {
    cam = camera;
    screenScale.x = (canvasDim.w/cam.get_width());
    screenScale.y = (canvasDim.h/cam.get_height());

}

void RenderBuffer::debug_mode(bool mode) {
    debug = mode;
}

SDL_Vertex* RenderBuffer::data() {
    return buffer.data();
}

void RenderBuffer::clear() {
    batches.clear();
    buffer.clear();
    current_batch.start_index = buffer.size();
    current_batch.texture_id = 0;
    loaded = false;
}

void RenderBuffer::resize(int num) {
    buffer.resize(num);
}

void RenderBuffer::pop_last_batch() {
    current_batch.num_vertices = size() - current_batch.start_index;
    batches.push_back(current_batch);
}

