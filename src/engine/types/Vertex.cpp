#include "../../../include/engine/types/positioning/Vertex.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

#include "../../../include/engine/wip/Random.hpp"

using namespace geng;

void Vertex::set(int xpos, int ypos, int zpos) {
	x = xpos;
	y = ypos;
	z = zpos;
}

Vertex Vertex::unit() const {
	float m = mag();
	if (m < 1e-6f)
		return {0.f, 0.f, 0.f};
	return {x / m, y / m, z / m};
}

float Vertex::mag() const {
	return std::sqrt((x*x) + (y*y) + (z*z));
}

float Vertex::xy_mag() const {
	return std::sqrt((x*x) + (y*y));
}

float Vertex::dist(Vertex v) const {
	float xdis = x - v.x;
	float ydis = y - v.y;
	float zdis = z - v.z;
	return (std::sqrtf((xdis*xdis) + (ydis*ydis) + (zdis*zdis)));
}

void Vertex::to_zero() {
	set(0.f,0.f,0.f);
}

bool Vertex::is_zero() const {
	return x==0 && y==0 && z==0;
}

void Vertex::randomize(std::pair<float, float> range)  {
	if (range.first == 0.f && range.second == 0.f) {
		range.first = -1.f;
		range.second = 1.f;
	}
	x = (random() * (random()%2 ? -1 : 1));
	y = (random() * (random()%2 ? -1 : 1));
	z = (random() * (random()%2 ? -1 : 1));
}

Vertex Vertex::abs() {
	return {fabsf(x), fabsf(y), fabsf(z)};
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
	if (std::fabs(scalar) < 1e-6f)
		return {0.f, 0.f, 0.f};
	return {x / scalar, y / scalar, z / scalar};
}

Vertex Vertex::operator%(int scalar) const {
	return Vertex(static_cast<int>(x)%scalar, // NOLINT(*-return-braced-init-list)
		static_cast<int>(y)%scalar,
		static_cast<int>(z)%scalar);
}

void Vertex::operator+=(Vertex other){
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

void Vertex::operator%=(int scalar) {
	x = static_cast<int>(x)%scalar;
	y = static_cast<int>(y)%scalar;
	z = static_cast<int>(z)%scalar;
}

float Vertex::operator*(Vertex other) {
	return x*other.x + y*other.y + z*other.z;
}

void Vertex::operator/=(float scalar){
	x /= scalar;
	y /= scalar;
	z /= scalar;
}

float& Vertex::operator[](int index) {
	assert(index >= 0 && index < 3);
	return (&x)[index];
}

void Vertex::operator=(Vertex other) {
	x = other.x;
	y = other.y;
	z = other.z;
}

std::string Vertex::operator+(std::string s) const {
	return to_string() + s;
}

std::string Vertex::operator<<(std::string s) const {
	return to_string() + s;
}

std::string Vertex::to_string() const {
	return "{"	+ std::to_string(x)
				+ "," + std::to_string(y)
				+ "," + std::to_string(z)
				+ "}";
}

bool Vertex::operator==(Vertex other) const {
	constexpr float eps = 0.001f;
	return std::fabs(x - other.x) < eps &&
		   std::fabs(y - other.y) < eps &&
		   std::fabs(z - other.z) < eps;
}

bool Vertex::is_finite() const {
	return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
}

void Vertex::sanitize() {
	if (!std::isfinite(x)) x = 0.f;
	if (!std::isfinite(y)) y = 0.f;
	if (!std::isfinite(z)) z = 0.f;
}