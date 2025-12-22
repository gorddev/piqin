#include "../../../../include/engine/types/positioning/FPos3D.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

#include "../../../../include/engine/wip/Random.hpp"

using namespace geng;

void FPos3D::set(float xpos, float ypos, float zpos) {
	x = xpos;
	y = ypos;
	z = zpos;
}

FPos3D FPos3D::unit() const {
	float m = mag();
	if (m < 1e-6f)
		return {0.f, 0.f, 0.f};
	return {x / m, y / m, z / m};
}

float FPos3D::mag() const {
	return std::sqrt((x*x) + (y*y) + (z*z));
}

float FPos3D::xy_mag() const {
	return std::sqrt((x*x) + (y*y));
}

float FPos3D::dist(FPos3D v) const {
	float xdis = x - v.x;
	float ydis = y - v.y;
	float zdis = z - v.z;
	return (std::sqrtf((xdis*xdis) + (ydis*ydis) + (zdis*zdis)));
}

void FPos3D::to_zero() {
	set(0.f,0.f,0.f);
}

bool FPos3D::is_zero() const {
	return x==0 && y==0 && z==0;
}

void FPos3D::randomize(std::pair<float, float> range)  {
	if (range.first == 0.f && range.second == 0.f) {
		range.first = -1.f;
		range.second = 1.f;
	}
	x = (random() * (random()%2 ? -1 : 1));
	y = (random() * (random()%2 ? -1 : 1));
	z = (random() * (random()%2 ? -1 : 1));
}

FPos3D FPos3D::abs() {
	return {fabsf(x), fabsf(y), fabsf(z)};
}

FPos3D FPos3D::operator+(FPos3D other) const {
	return {x + other.x, y + other.y, z + other.z};
}

FPos3D FPos3D::operator-(FPos3D other) const {
	return FPos3D(x - other.x, y - other.y, z - other.z);
}

FPos3D FPos3D::operator*(float scalar) const {
	return FPos3D(x* scalar, y*scalar, z* scalar);
}

FPos3D FPos3D::operator/(float scalar) const {
	if (std::fabs(scalar) < 1e-6f)
		return {0.f, 0.f, 0.f};
	return {x / scalar, y / scalar, z / scalar};
}

FPos3D FPos3D::operator%(int scalar) const {
	return FPos3D(static_cast<int>(x)%scalar, // NOLINT(*-return-braced-init-list)
		static_cast<int>(y)%scalar,
		static_cast<int>(z)%scalar);
}

void FPos3D::operator+=(FPos3D other){
	x += other.x;
	y += other.y;
	z += other.z;
}

void FPos3D::operator-=(FPos3D other){
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void FPos3D::operator*=(float scalar){
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void FPos3D::operator%=(int scalar) {
	x = static_cast<int>(x)%scalar;
	y = static_cast<int>(y)%scalar;
	z = static_cast<int>(z)%scalar;
}

float FPos3D::operator*(FPos3D other) {
	return x*other.x + y*other.y + z*other.z;
}

void FPos3D::operator/=(float scalar){
	x /= scalar;
	y /= scalar;
	z /= scalar;
}

float& FPos3D::operator[](int index) {
	assert(index >= 0 && index < 3);
	return (&x)[index];
}

void FPos3D::operator=(FPos3D other) {
	x = other.x;
	y = other.y;
	z = other.z;
}

std::string FPos3D::operator+(std::string s) const {
	return to_string() + s;
}

std::string FPos3D::operator<<(std::string s) const {
	return to_string() + s;
}

std::string FPos3D::to_string() const {
	return "{"	+ std::to_string(x)
				+ "," + std::to_string(y)
				+ "," + std::to_string(z)
				+ "}";
}

bool FPos3D::operator==(FPos3D other) const {
	constexpr float eps = 0.001f;
	return std::fabs(x - other.x) < eps &&
		   std::fabs(y - other.y) < eps &&
		   std::fabs(z - other.z) < eps;
}

bool FPos3D::is_finite() const {
	return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
}

void FPos3D::sanitize() {
	if (!std::isfinite(x)) x = 0.f;
	if (!std::isfinite(y)) y = 0.f;
	if (!std::isfinite(z)) z = 0.f;
}