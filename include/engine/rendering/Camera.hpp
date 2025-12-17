#pragma once
#include "engine/types/Vertex.hpp"

namespace geng {
	/** The Camera that controls the scene. Currently, it does not affect rendering in any capacity */
	class Camera{
	public:
		Vertex pos;
		int w = 0;
		int h = 0;

		// Camera constructor
		Camera() { }
		Camera(int x, int y, int z, int w, int h) : w(w), h(h) {
			pos.set(x,y,z);
		}
		// Getters
		[[nodiscard]] int width() const { return w; }       // Returns width
		[[nodiscard]] int height() const { return h; }      // Returns height
		// Center
		[[nodiscard]] Vertex center() const { return Vertex(pos.x + (w / 2.0), pos.y - (h / 2.0), pos.z); }

		// Setters
		void set_width(const int& width) { w = width; }
		void set_height(const int& height) { h = height; }

		// Gets the gamera position
	};
}
