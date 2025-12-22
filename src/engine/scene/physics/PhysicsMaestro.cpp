#include "engine/scene/physics/PhysicsMaestro.hpp"

#include "engine/scene/physics/TileContact.hpp"
#include "engine/types/positioning/FBox2D.hpp"
#include "engine/utilities/Utilities.hpp"

using namespace geng;



PhysicsMaestro::PhysicsMaestro(LayerContext& scene, WorldManager &world)
    : world(world), scene(scene) {}

void PhysicsMaestro::add_collider(Collider *collider) {
    /// Add the collider
    colliders.push_back(collider);
    // Also add their previous position
    prev_positions.push_back(collider->get_position());
}

void PhysicsMaestro::remove_collider(Collider *collider) {
    // Make sure to erase the previous position too
    auto prev = prev_positions.begin();
    // Go through our vector of colliders
    for (auto col = colliders.begin(); col != colliders.end(); ++col, ++prev) {
        // If col == collider
        if (*col == collider) {
            // erase from both colliders and previous positions
            colliders.erase(col);
            prev_positions.erase(prev);
            break;
        }
    }
}

void PhysicsMaestro::load_current_level() {
    delete grid;
    grid = new TileGrid(world.get_current_level(), world.get_tile_size());
}

void PhysicsMaestro::update() {
    if (grid != nullptr) {
        TileGrid g = *grid;
        for (int i = 0; i < colliders.size(); ++i) {
            collide(g, i);
        }
    }
}

std::vector<Collider *> & PhysicsMaestro::get_colliders() {
    return colliders;
}

void PhysicsMaestro::render_hitboxes(RenderBuffer &buffer, SDL_Color color, uint16_t thickness) {
    if (grid != nullptr) {
        std::vector<SDL_FPoint> points = grid->display_hitboxes(thickness);
        buffer.push_back(points, color);
    }
}

inline bool tile_overlap(const FBox2D& a, const FBox2D& b) {
    return !(a.x + a.w <= b.x ||
             a.x >= b.x + b.w ||
             a.y + a.h <= b.y ||
             a.y >= b.y + b.h);
}


void PhysicsMaestro::collide(TileGrid& grid, int& id) {
    Collider& c = *colliders[id];

    FPos2D start = prev_positions[id];
    FPos2D vel = c.t.pos - FPos2D{c.get_hitbox().w/2.f - c.get_offset().x, c.get_hitbox().h/2.f - c.get_offset().y} - start; // total intended movement
    Dim2D box = c.get_hitbox();
    uint16_t tile_type = 0;

    if (vel.x == 0 && vel.y == 0) {
        prev_positions[id] = start;
        return;
    }

    // we step multiple times and detect collisions then. our max step is half of a tile
    float maxStep = static_cast<float>(grid.tileSize) * 0.5f;
    int stepsX = static_cast<int>(std::ceil(std::abs(vel.x) / maxStep));
    int stepsY = static_cast<int>(std::ceil(std::abs(vel.y) / maxStep));
    int steps = std::max(stepsX, stepsY);
    steps = std::max(1, steps);

    FPos2D stepDelta = vel / static_cast<float>(steps);
    FPos2D pos = start;
    bool collision = false;

    // Go through each of our steps
    for (int i = 0; i < steps; ++i) {
        // ***********************
        // x-axis
        // ************************
        pos.x += stepDelta.x;

        int min_tx = std::max(0, static_cast<int>(std::floor(pos.x / grid.tileSize)));
        int max_tx = std::min(grid.width - 1, static_cast<int>(std::floor((pos.x + box.w) / grid.tileSize)));
        int min_ty = std::max(0, static_cast<int>(std::floor(pos.y / grid.tileSize)));
        int max_ty = std::min(grid.height - 1, static_cast<int>(std::floor((pos.y + box.h) / grid.tileSize)));

        for (int ty = min_ty; ty <= max_ty; ++ty) {
            for (int tx = min_tx; tx <= max_tx; ++tx) {
                if (grid.at(tx, ty) == 0) continue;

                FBox2D tile = grid.tile_box(tx, ty);
                FBox2D mover { pos.x, pos.y, box.w, box.h };

                if (!tile_overlap(mover, tile)) continue;

                // Push out along X
                if (stepDelta.x > 0) pos.x = tile.x - box.w;
                else if (stepDelta.x < 0) pos.x = tile.x + tile.w;

                stepDelta.x = 0;
                collision = true;
                tile_type = grid.at(tx, ty);
            }
        }

        // ***********************
        // x-axis
        // ************************
        pos.y += stepDelta.y;

        min_tx = std::max(0, static_cast<int>(std::floor(pos.x / grid.tileSize)));
        max_tx = std::min(grid.width - 1, static_cast<int>(std::floor((pos.x + box.w) / grid.tileSize)));
        min_ty = std::max(0, static_cast<int>(std::floor(pos.y / grid.tileSize)));
        max_ty = std::min(grid.height - 1, static_cast<int>(std::floor((pos.y + box.h) / grid.tileSize)));

        for (int ty = min_ty; ty <= max_ty; ++ty) {
            for (int tx = min_tx; tx <= max_tx; ++tx) {
                if (grid.at(tx, ty) == 0) continue;

                FBox2D tile = grid.tile_box(tx, ty);
                FBox2D mover { pos.x, pos.y, box.w, box.h };

                if (!tile_overlap(mover, tile)) continue;

                // collide along y
                if (stepDelta.y > 0) {
                    pos.y = tile.y - box.h;
                }
                else if (stepDelta.y < 0) {
                    pos.y = tile.y + tile.h;
                }

                stepDelta.y = 0;
                collision = true;
                tile_type = grid.at(tx, ty);
            }
        }
    }

    // apply the final updated position and velocity.
    if (collision) {
        FPos2D newPos = pos + FPos2D{c.get_hitbox().w/2.f - c.get_offset().x, c.get_hitbox().h/2.f - c.get_offset().y};
        ProposedCollision p = {newPos, newPos - start, tile_type, collision};
        c.collide(p);
    }
    prev_positions[id] = pos;

}