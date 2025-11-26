#pragma once
#include "textures/SpriteMap.hpp"
#include "utilities/BinaryDecoder.hpp"
#include <vector>

// Contains the structure of a given bitmap to load into sprites
struct BitmapInfo {
	// Contains the number of frames in a sprite
	int numFrames;
	// Contains the position and dimensions of every frame.
	std::vector<Frame> frameInfo;
	// Gives the path to the .bmp file
	string bitmap;

	// Constructors
	BitmapInfo() : numFrames(0), bitmap("") {frameInfo.reserve(1);}
	BitmapInfo(int numFrames, string bitmap) : numFrames(numFrames), bitmap(bitmap) {frameInfo.reserve(numFrames);}

	// Given a file representing
	void loadFrameInfo(string filePath);

};