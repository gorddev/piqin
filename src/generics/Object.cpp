#include "../../include/generics/Object.hpp"

#include <print>

#include "Constants.hpp"
#include "textures/Frame.hpp"


Object::~Object() {
	delete tex_render_info;
}
#include <iostream>
// Updates position if the velocity vertex is nonzero.
void Object::update_pos() {
	fixed = false;

	// Update if we're currently shaking. If we don't have path priority.
	if (!pathPriority && shake != nullptr) {
		// If we end the shake, remove the shake.
		if (shake->shake_it(t.pos)) {
			delete shake;
			shake = nullptr;
		}
	}
	// If our path is still here
	else if (path != nullptr) {
		// We move the path along
		path->to_path(t.pos);
		// If we complete the path
		if (path->path_complete()) {
			// Delete it and reset pathPriority
			delete path;
			path = nullptr;
			pathPriority = false;
			// If our shake is not null, then we update it with a target pos.
			if (shake != nullptr)
				shake->set_pos_pop(t.pos);
		}
	}
}

// Getters
Vertex Object::pos() const { return t.pos; }
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

Shake * Object::get_shake() const {
	return shake;
}

// Setters
void Object::set_pos(const Vertex &v) {
	fixed = false;
	t.pos = v;
}

void Object::add_pos(const Vertex &v) {
	t.pos += v;
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
	if (pathPriority)
		pathPriority = !pathPriority;
	else if (shake == nullptr)
		pathPriority = true;
	path = new Path(p);
}

void Object::set_path(Vertex target, uint8_t pathType,
	float speed) {
	fixed = false;
	delete path;
	// If we don't have a shake, the path gets priority
	if (pathPriority)
		pathPriority = !pathPriority;
	else if (shake == nullptr)
		pathPriority = true;
	// Now we update our position return for the shake.
	path = new Path({target, pos(), pathType, speed});
}

void Object::set_shake(uint8_t shakeType, float strength,
	int duration, float speed, bool decay) {
	Vertex v = (shake != nullptr) ? shake->pop_pos() : t.pos;
	delete shake;
	shake = new Shake(shakeType, strength, duration, speed, decay);
	std::cerr << v.to_string() + "\n";
	shake->set_pos_pop(v);
}

void Object::set_shake(const Shake &s) {
	Vertex v = (shake != nullptr) ? shake->pop_pos() : t.pos;
	delete shake;
	shake = new Shake(s);
	shake->set_pos_pop(v);
}


string Object::to_string() {
	return	"pos: "	+ t.pos.to_string() +
			"\nscale: " + std::to_string(t.scale);
}

int Object::increment_frameNum() {
	fixed = false;
	return ++fs.frameNum;
}
