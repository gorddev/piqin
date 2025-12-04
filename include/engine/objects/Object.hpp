#pragma once

#include <SDL_render.h>

#include <utility>
#include "engine/objects/Path.hpp"
#include "engine/objects/Shake.hpp"
#include "engine/objects/Transform.hpp"
#include "../gengine-globals/EngineEnums.hpp"
#include "engine/textures/FrameState.hpp"

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
        // Contains framenum, duration, ect.
        FrameState fs;
        // Contains pos, scale, shake_offset, ect.
        Transform t;
    protected:
        // Allows us to access Path information
        Path* path = nullptr;
        // Allows us to access a Shake variable
        Shake* shake = nullptr;
    public:
        // Constructors
        Object() = default;
        // ReSharper disable once CppPassValueParameterByConstReference
        explicit Object(Vertex v) : t(v) {}
        Object(Vertex v, FrameState* spr) : t(v) {}
        Object(Object& o) : id(o.id), fixed{o.fixed}, fs(o.fs), t(o.t) {}
        // Destructor
        virtual ~Object();

        // Member functions
        void update_pos();

        bool update_anim();

        // Getters
        // Transform
        [[nodiscard]] Vertex pos() const;
        [[nodiscard]] Vertex offset() const;
        [[nodiscard]] float x() const;
        [[nodiscard]] float y() const;
        [[nodiscard]] float z() const;

        float *zptr();

        [[nodiscard]] float scale() const;
        [[nodiscard]] int height() const;
        [[nodiscard]] int width() const;
        [[nodiscard]] float rotation() const;
        [[nodiscard]] bool flip() const;
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
        void set_flip(bool flippy);
        // Frame State
        void set_animation(uint8_t new_animation);
        // Path, shaking, ect.
        void set_path(const Path &p, bool priority = false);
        void set_path(Vertex target, GENG_Path pathType, float speed, bool priority = false);
        void set_shake(GENG_Shake shakeType, float strength, int duration, float speed = 1.0, bool decay = true);
        void set_shake(const Shake& s);
        void remove_shake();

        // To String
        [[nodiscard]] std::string to_string() const;
    };
}
