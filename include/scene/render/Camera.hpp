#pragma once
#include "generics/Vertex.hpp"
#include "Constants.hpp"
#include "generics/Path.hpp"

class Camera{
public:
	Vertex pos;
	int w = scene::width;
	int h = scene::height;

	// Camera constructor
	Camera() {pos.z = ZTOP;}
	Camera(int x, int y, int z, int w, int h) : w(w), h(h) {
		pos.set(x,y,z);
	}
	// Getters
	[[nodiscard]] int width() const { return w; }       // Returns width
	[[nodiscard]] int height() const { return h; }      // Returns height
	// Center
	[[nodiscard]] Vertex center() const { return {pos.x + (w / 2.0), pos.y - (h / 2.0), pos.z}; }

	// Setters
	void set_width(const int& width) { w = width; }
	void set_height(const int& height) { h = height; }

	// Gets the gamera position
};
