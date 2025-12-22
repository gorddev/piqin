#pragma once
#include "TileContact.hpp"
#include "engine/scene/sprites/Sprite.hpp"
#include "engine/types/positioning/Box2D.hpp"

namespace geng {

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
        Dim2D hitbox;
        /// Hitbox offset for calculation purposes
        Pos2D offset;
        /// Velocity of the collider
        FPos2D vel;
    public:
        /// Must specify frametable and hitbox for a collider object
        explicit Collider(FrameTable& frames, Dim2D hitbox) : Sprite(frames), hitbox(hitbox) {}
        /// Also allows for construction of width and height:
        explicit Collider(FrameTable& frames, Transform2D t, Dim2D hitbox, Pos2D offset = {0, 0}) : Sprite(frames, t), hitbox(hitbox), offset(offset) {}

        /// @brief Determines what to do upon collision with any given tile.
        /// @param collision › the box the object collides with
        /// @param tile_type › the type of tile the collider collides with
        virtual void collide(ProposedCollision& collision) {
            std::cerr << "collision with tile: " << collision.tile_type << "\n";
            t.pos = collision.newPos;
            vel = collision.delta;
        }

        /// Gets the position of the Collider
        FPos2D get_position() { return t.pos; }
        /// Gets the velocity of a collider
        FPos2D get_velocity() { return vel; }
        /// Gets a representative box of the collider
        Dim2D get_hitbox() { return hitbox; }
        /// gets the offset of the collider
        Pos2D get_offset() { return offset; }

        /// Sets the velocity
        void set_velocity(FPos2D v) { vel = v; }

        /// Turns the collider into a hitbox
        std::vector<SDL_FPoint> to_vertex_hitbox(uint16_t thickness) {
            std::vector<SDL_FPoint> points = Box2D(t.pos.x - hitbox.w/2 + offset.x, t.pos.y - hitbox.h/2 + offset.y, hitbox.w, hitbox.h).to_vertex_hitbox(thickness);
            return points;
        }
    };
}
