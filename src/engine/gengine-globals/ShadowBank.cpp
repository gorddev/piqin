#include "../../../include/engine/rendering/ShadowBank.hpp"

geng::ShadowBank& geng::get_shadow_calc() {
    static ShadowBank shadows;
    return shadows;
}

void geng::shadow_background(std::vector<SDL_Vertex>& buffer, int& numVertices) {
    // First gets our size before we update with new buffer
    int oldsize = static_cast<int>(buffer.size());
    // Resize our vector accordingly.

    buffer.resize(oldsize + numVertices);

    // Now we copy the old memory to the new positions
    // First we get the location of the new data
    SDL_Vertex* loc = buffer.data() + oldsize;
    // And the size of each SDL_Vertex is 24, so we have 24*numVertices bytes to copy
    // Then we memcpy them cause it's fast
    std::memcpy(loc, loc - numVertices, numVertices*sizeof(SDL_Vertex));
    // Finally we update each vertex location
    for (int i = oldsize - numVertices; i < oldsize; i++) {
        // Store vertex in reference for easy access
        SDL_Vertex& vert = buffer[i];
        // Update its position
        vert.position = {
            vert.position.x + 0.035f*(vert.position.x - (200*0.5f)),
            vert.position.y + (0.035f*(vert.position.y - 200*0.25f))
        };
        // Update it's color
        vert.color = {0, 0, 0, 50};
    }

}

void geng::shadow_floor(std::vector<SDL_Vertex>& buffer, int& numVertices) {

}