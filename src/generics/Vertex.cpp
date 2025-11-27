#include "generics/Vertex.hpp"
#include <cmath>
#include <iostream>

void Vertex::set(int xpos, int ypos, int zpos) {
	x = xpos;
	y = ypos;
	z = zpos;
}

Vertex Vertex::unit() const {
	double maggy = mag();
	if (maggy == 0)
		return {0,0,0};
	return {x / maggy, y / maggy, z / maggy};
}

double Vertex::mag() const {
	return std::sqrt((x*x) + (y*y) + (z*z));
}

double Vertex::xy_mag() const {
	return std::sqrt((x*x) + (y*y));
}

double Vertex::dist(Vertex v) const {
	int xdis = x - v.x;
	int ydis = y - v.y;
	int zdis = z - v.z;
	return (std::sqrt((xdis*xdis) + (ydis*ydis) + (zdis*zdis)));
}

bool Vertex::zero() const {
	return x==0 && y==0 && z==0;
}

Vertex Vertex::operator+(Vertex other) const {
	return {x + other.x, y + other.y, z + other.z};
}

Vertex Vertex::operator-(Vertex other) const {
	return Vertex(x - other.x, y - other.y, z - other.z);
}

Vertex Vertex::operator*(float scalar) const {
	return Vertex(x* scalar, y*scalar, z* scalar);
}

Vertex Vertex::operator/(float scalar) const {
	return Vertex(x/ scalar, y/scalar, z/scalar);
}

void Vertex::operator+=(Vertex other){
	std::cerr << "other: " + other.to_string() + "\n";
	std::cerr<< "this: " + to_string() + "\n";
	x += other.x;
	y += other.y;
	z += other.z;
}

void Vertex::operator-=(Vertex other){
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void Vertex::operator*=(float scalar){
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

float Vertex::operator*(Vertex other) {
	return x*other.x + y*other.y + z*other.z;
}

void Vertex::operator/=(float scalar){
	x /= scalar;
	y /= scalar;
	z /= scalar;
}

double& Vertex::operator[](int index) {
	if (!index) return x;
	if (index == 1) return y;
	if (index == 2) return z;
	std::cerr << "Vertex::ERR: Index doesn't exist. Returning x.\n";
	return x;
}

void Vertex::operator=(Vertex other) {
	x = other.x;
	y = other.y;
	z = other.z;
}

std::string Vertex::operator+(std::string s) {
	return to_string() + s;
}

std::string Vertex::to_string() const {
	return "{"	+ std::to_string(x)
				+ "," + std::to_string(y)
				+ "," + std::to_string(z)
				+ "}";
}

bool Vertex::operator==(Vertex other) const {
	int tempx = (x - other.x)*1000;
	int tempy = (y - other.y)*1000;
	int tempz = (z - other.z)*1000;
	return (!tempx && !tempy && !tempz);
}