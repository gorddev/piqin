#include "../../include/generics/Object.hpp"

#include <print>

#include "Constants.hpp"
#include "textures/Frame.hpp"


Object::~Object() {
	delete tex_render_info;
}
#include <iostream>
// Updates position if the velocity vertex is nonzero.
void Object::update_pos(double dt) {
	fixed = false;
	if (path != nullptr) {
		path->to_path(t.pos);
		if (path->path_complete()) {
			delete path;
			path = nullptr;
		}
	}
	else
	t.pos += t.vel * VEL_SCALE * dt;

}

// Getters
Vertex Object::pos() const { return t.pos; }
Vertex Object::velocity() const { return t.vel; }
double Object::x() const { return t.pos.x; }
double Object::y() const { return t.pos.y; }
double Object::z() const { return t.pos.z; }
float Object::scale() const { return t.scale; }
int Object::sheet_id() const { return fs.sheet_id; }
int Object::frameNum() const { return fs.frameNum; }
uint8_t Object::state() const { return fs.state; }
FrameState* Object::frame_state() { return &fs; }

int Object::height() const {
	return t.height;
}

int Object::width() const {
	return t.width;
}

int & Object::duration() {
	return fs.duration;
}

float Object::get_z_index() {
	return t.z_index;
}

Path* Object::get_path() const {
	return path;
}

// Setters
void Object::set_pos(const Vertex &v) {
	fixed = false;
	t.pos = v;
}

void Object::add_pos(const Vertex &v) {
	t.pos += v;
}

void Object::set_velocity(const Vertex &v) {
	fixed = false;
	t.vel = v;
}
void Object::set_x(const int new_x) {
	fixed = false;
	t.pos.x = new_x;
}
void Object::set_y(const int new_y) {
	fixed = false;
	t.pos.y = new_y;
}
void Object::set_z(const int new_z) {
	fixed = false;
	t.pos.z = new_z;
}
void Object::set_scale(const float scale) {
	fixed = false;
	t.scale = scale;
}
void Object::set_sheet_id(const int new_sheet_id) {
	fixed = false;
	fs.sheet_id = new_sheet_id;
}

void Object::set_frame_number(int new_frame_number) {
	fixed = false;
	fs.frameNum = new_frame_number;
}

void Object::set_state(uint8_t new_state) {
	fixed = false;
	fs.state = new_state;
}

void Object::set_status(uint8_t new_status) {
	fixed = false;
	fs.state = new_status;
}

void Object::set_height(int new_height) {
	fixed = false;
	t.height = new_height;
}

void Object::set_width(int new_width) {
	fixed = false;
	t.width = new_width;
}

void Object::set_z_index(float new_z_index) {
	fixed = false;
	t.z_index = new_z_index;
}

void Object::set_path(const Path &p) {
	fixed = false;
	delete path;
	path = new Path(p);
}

void Object::set_path(Vertex target, uint8_t pathType,
	float speed) {
	fixed = false;
	delete path;
	path = new Path({target, pos(), pathType, speed});
}


string Object::to_string() {
	return	"pos: "	+ t.pos.to_string() +
			"\nvel: "	+ t.vel.to_string() +
			"\nscale: " + std::to_string(t.scale);
}

int Object::increment_frameNum() {
	fixed = false;
	return ++fs.frameNum;
}
