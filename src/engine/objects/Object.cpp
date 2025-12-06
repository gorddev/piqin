#include "engine/objects/Object.hpp"

#include <iostream>

#include "engine/gengine-globals/scene.hpp"

using namespace gengine;

Object::~Object() {
	delete shake;
	delete path;
}

// Updates position if the velocity vertex is nonzero.
void Object::update_pos() {
	fixed = false;

	// Update if we're currently shaking.
	if (shake != nullptr) {
		// If we end the shake, remove the shake
		t.offset = shake->shake_it();
		if (shake->done()) {
			t.offset.set(0,0,0);
			delete shake;
			shake = nullptr;
		}
	}
	// If our path is still here
	if (path != nullptr) {
		// We move the path along
		path->to_path(t.pos);
		// If we complete the path
		if (path->path_complete()) {
			// Delete it and reset pathPriority
			delete path;
			path = nullptr;
			pathPriority = false;
		}
	}
}

bool Object::update_anim() {
	return fs.update();
}

// Getters
Vertex Object::pos() const { return t.pos; }

Vertex Object::offset() const {
	return t.offset;
}

float Object::x() const { return t.pos.x; }
float Object::y() const { return t.pos.y; }
float Object::z() const { return t.pos.z; }
float* Object::zptr()  { return &t.pos.z; }
float Object::scale() const { return t.scale; }

int Object::height() const {
	return t.h;
}

int Object::width() const {
	return t.w;
}

float Object::rotation() const {
	return t.angle;
}

bool Object::flip() const {
	return t.flipX;
}

float& Object::duration() {
	return fs.duration;
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

void Object::set_x(const float new_x) {
	fixed = false;
	t.pos.x = new_x;
}
void Object::set_y(const float new_y) {
	fixed = false;
	t.pos.y = new_y;
}
void Object::set_z(const float new_z) {
	fixed = false;
	t.pos.z = new_z;
}
void Object::set_scale(const float scale) {
	fixed = false;
	t.scale = scale;
}

void Object::set_animation(uint8_t new_animation) {
	fs.dirty = true;
	fs.duration = 0;
	fs.animation_index = new_animation;
}

void Object::set_height(int new_height) {
	fixed = false;
	t.h = new_height;
}

void Object::set_width(int new_width) {
	fixed = false;
	t.w = new_width;
}

void Object::set_flip(bool flippy) {
	fixed = false;
	t.flipX = flippy;
}


void Object::set_path(const Path &p, bool priority) {
	tag = GENG_Tag::NONE;
	fixed = false;
	delete path;
	if (shake == nullptr || priority)
		pathPriority = true;
	path = new Path(p);
}

void Object::set_path(Vertex target, GENG_Path pathType,
	float speed, bool priority) {
	tag = GENG_Tag::NONE;
	fixed = false;
	delete path;
	// If we don't have a shake, the path gets priority
	if (shake == nullptr || priority)
		pathPriority = true;
	// Now we update our position return for the shake.
	path = new Path({target, pos(), pathType, speed});
}


void Object::set_shake(GENG_Shake GENG_Shake, float strength,
                       int duration, float speed, bool decay) {
	delete shake;
	shake = new Shake(GENG_Shake, strength, duration, speed, decay);
}

void Object::set_shake(const Shake &s) {
	delete shake;
	shake = new Shake(s);
}


void Object::remove_shake() {
	t.offset.set(0,0,0);
	delete shake;
	shake = nullptr;
}


std::string Object::to_string() const {
	return	"pos: "	+ t.pos.to_string() +
			"\nscale: " + std::to_string(t.scale) +
			"\npath: " + (path ==nullptr ? "nullptr" : path->to_string()) +
			"\nshake: " + ((shake==nullptr) ? "nullptr" : "tostring:\n" + shake->get_displacement().to_string()) +
			"\nid: " + std::to_string(id) +
			"\nsheet_id: " + std::to_string(fs.frame_sheet_id) +
				"\nflag: " + flag + "\n";
}