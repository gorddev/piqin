#pragma once
#include "render/TextureRenderInfo.hpp"
#include "generics/FrameState.hpp"
#include "generics/Transform.hpp"
#include "generics/Path.hpp"
#include <string>

#include "Shake.hpp"

using std::string;

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
    uint8_t tag = NULL;
    // Let's us flag output for specific objects going thru object manager
    string flag;
    // Contains info for rendering
    TextureRenderInfo* tex_render_info = nullptr;
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
    Object(Object& o) : t(o.t), fs(o.fs), id(o.id), fixed{o.fixed}, tex_render_info(o.tex_render_info) {}
    // Destructor
    virtual ~Object();

    // Member functions
    void update_pos();

    // Getters
    [[nodiscard]] Vertex pos() const;
    [[nodiscard]] double x() const;
    [[nodiscard]] double y() const;
    [[nodiscard]] double z() const;
    [[nodiscard]] float scale() const;
    [[nodiscard]] int sheet_id() const;
    [[nodiscard]] int frameNum() const;
    [[nodiscard]] uint8_t state() const;
    [[nodiscard]] FrameState* frame_state();
    [[nodiscard]] int height() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] float& duration();
    [[nodiscard]] float get_z_index();
    [[nodiscard]] Path *get_path() const;
    [[nodiscard]] Shake* get_shake() const;

    // Setters
    void set_pos(const Vertex &v);
    void add_pos(const Vertex &v);
    void set_x(int new_x);
    void set_y(int new_y);
    void set_z(int new_z);
    void set_scale(float scale);
    void set_sheet_id(int new_sheet_id);
    void set_frame_number(int new_frame_number);
    void set_state(uint8_t new_state);
    void set_status(uint8_t new_status);
    void set_height(int new_height);
    void set_width(int new_width);
    void set_z_index(float new_z_index);
    void set_path(const Path &p, bool priority = false);
    void set_path(Vertex target, uint8_t pathType, float speed, bool priority = false);
    void set_shake(uint8_t shakeType, float strength, int duration, float speed = 1.0, bool decay = true);
    void set_shake(const Shake& s);
    void remove_shake();

    // To String
    string to_string();

    // Increment
    int increment_frameNum();
};