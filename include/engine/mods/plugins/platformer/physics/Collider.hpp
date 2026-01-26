#pragma once
#include "TileContact.hpp"
#include "engine/mods/animation/sprites/Sprite.hpp"
#include "engine/types/positioning/fbox2.hpp"

namespace gan {

    /** @brief Collider is a special kind of sprite that has a velocity and a hitbox.
     * They are the only type that is allowed to feed into the physics engine.
     * @details Colliders have two member variables:
     * - @code Dim2D hitbox@endocde › The hitbox they use for collisions
     * - @code FPos2D vel@endcode › Velocity of the collider
     * - ----- Member functions: ----------
     */
    class Collider : public Sprite {
    protected:
        /// Hitbox of the collider
        dim2 hitbox;
        /// Hitbox offset for calculation purposes
        pos2 offset;
        /// Velocity of the collider
        vec2 vel;
    public:
        /// Must specify frametable and hitbox for a collider object
        explicit Collider(FrameTable& frames, dim2 hitbox) : Sprite(frames), hitbox(hitbox) {}
        /// Also allows for construction of width and height:
        explicit Collider(FrameTable& frames, Transform2D t, dim2 hitbox, pos2 offset = {0, 0}) : Sprite(frames, t), hitbox(hitbox), offset(offset) {}

        /// @brief Determines what to do upon collision with any given tile.
        /// @param collision › the box the object collides with
        /// @param tile_type › the type of tile the collider collides with
        virtual void collide(ProposedCollision& collision) {
            t.pos = collision.newPos;
        }

        /// Gets the position of the Collider
        vec2 get_position() { return t.pos; }
        /// Gets the velocity of a collider
        vec2 get_velocity() { return vel; }
        /// Gets a representative box of the collider
        dim2 get_hitbox() { return hitbox; }
        /// gets the offset of the collider
        pos2 get_offset() { return offset; }

        /// Sets the velocity
        void set_velocity(vec2 v) { vel = v; }

        /// Turns the collider into a hitbox
        std::vector<SDL_FPoint> to_vertex_hitbox(uint16_t thickness) {
            std::vector<SDL_FPoint> points = fbox2(t.pos.x - hitbox.w/2 + offset.x, t.pos.y - hitbox.h/2 + offset.y, hitbox.w, hitbox.h).to_hitbox(thickness);
            return points;
        }
    };
}
