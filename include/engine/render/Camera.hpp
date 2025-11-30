#pragma once
#include "../utilities/types/Vertex.hpp"
#include "../gengine-globals/scene.hpp"

namespace gengine {
	class Camera{
	public:
		Vertex pos;
		int w = glb::scene.width;
		int h = glb::scene.height;

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