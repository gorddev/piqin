#pragma once

#include <SDL_render.h>
#include "engine/objects/Path.hpp"
#include "engine/objects/Shake.hpp"
#include "engine/objects/Transform.hpp"
#include "engine/sprites/FrameState.hpp"
#include "../gengine-globals/EngineEnums.hpp"

namespace gengine {
    class Object {
    public:
        // This object's id
        int id = -1;
        // If we make a path before we make a shake, the path gets priority.
        bool pathPriority = false;
        // If an object is fixed, we don't need to update it
        bool fixed = true;
        // If an object is hidden
        bool hidden = false;
        // If we want to hide the shadow
        bool shadow = true;
        // Lets us tag objects for pathing
        GENG_Tag tag = GENG_Tag::NONE;
        // Let's us flag output for specific objects going thru object manager
        std::string flag;
    protected:
        // Contains pos, velocity, scale, ect.
        Transform t;
        // Contains framenum, duration, ect.
        FrameState fs;
        // Allows us to access Path information
        Path* path = nullptr;
        // Allows us to access a Shake variable
        Shake* shake = nullptr;

    public:
        // Constructors
        Object() = default;
        // ReSharper disable once CppPassValueParameterByConstReference
        Object(Vertex v) : t(v) {}
        Object(Object& o) : t(o.t), fs(o.fs), id(o.id), fixed{o.fixed} {}
        // Destructor
        virtual ~Object();

        // Member functions
        void update_pos();

        // Getters
        // Transform
        [[nodiscard]] Vertex pos() const;
        [[nodiscard]] float x() const;
        [[nodiscard]] float y() const;
        [[nodiscard]] float z() const;
        [[nodiscard]] float scale() const;
        [[nodiscard]] int height() const;
        [[nodiscard]] int width() const;
        [[nodiscard]] float rotation() const;
        [[nodiscard]] SDL_RendererFlip flip() const;
        // FrameState
        [[nodiscard]] int sheet_id() const;
        [[nodiscard]] int frameNum() const;
        [[nodiscard]] uint8_t state() const;
        [[nodiscard]] FrameState* frame_state();
        // Pathing/shaking
        [[nodiscard]] float& duration();
        [[nodiscard]] Path *get_path() const;
        [[nodiscard]] Shake* get_shake() const;


        // Setters
        // Transform
        void set_pos(const Vertex &v);
        void add_pos(const Vertex &v);
        void set_x(float new_x);
        void set_y(float new_y);
        void set_z(float new_z);
        void set_scale(float scale);
        void set_height(int new_height);
        void set_width(int new_width);
        void set_flip(SDL_RendererFlip flippy);
        // Frame State
        void set_sheet_id(int new_sheet_id);
        void set_frame_number(int new_frame_number);
        void set_state(uint8_t new_state);
        void set_status(uint8_t new_status);
        int increment_frameNum();
        // Path, shaking, ect.
        void set_path(const Path &p, bool priority = false);
        void set_path(Vertex target, PathType pathType, float speed, bool priority = false);
        void set_shake(ShakeType shakeType, float strength, int duration, float speed = 1.0, bool decay = true);
        void set_shake(const Shake& s);
        void remove_shake();

        // To String
        [[nodiscard]] virtual std::string to_string() const;
    };
}