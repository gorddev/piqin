#include "engine/types/positioning/vec2.hpp"

#include <cmath>

using namespace gan;

vec2 vec2::unit() {
    float norm = sqrtf(x * x + y * y);
    vec2 result(x/norm, y/norm);
    sanitize();
    return result;
}

float vec2::mag() const {
    return sqrtf(x * x + y * y);
}

float vec2::dist(vec2 p) const {
    return sqrtf((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
}

vec2 vec2::abs() {
    return {fabsf(x), fabsf(y)};
}

void vec2::to_zero() {
    x = 0; y = 0;
}

void vec2::randomize() {
    x = static_cast<float>(rand()) * ((random()%2) ? 1 : -1);
    y = static_cast<float>(rand()) * ((random()%2) ? 1 : -1);
}

void vec2::sanitize() {
    if (!std::isfinite(x)) x = 0.f;
    if (!std::isfinite(y)) y = 0.f;
}

void vec2::set(float new_x, float new_y) {
    x = new_x;
    y = new_y;
}

bool vec2::is_zero() const {
    return fabsf(x) < 1e-5 && fabsf(y) < 1e-5;
}

bool vec2::is_finite() const {
    return std::isfinite(x) && std::isfinite(y);
}

bool vec2::operator==(const vec2& p) const {
    return (fabsf(x - p.x) < 1e-6 && fabsf(y - p.y) < 1e-6);
}
bool vec2::operator!=(const vec2& p) const {
    return !(*this == p);
}

vec2 vec2::operator+(const vec2& p) const {
    return {x + p.x, y + p.y};
}
vec2 vec2::operator-(const vec2& p) const {
    return {x - p.x, y - p.y};
}

vec2 vec2::operator*(const float scalar) const {
    return {x * scalar, y * scalar};
}

vec2 vec2::operator/(float scalar) const {
    vec2 result(x / scalar, y / scalar);
    result.sanitize();
    return result;
}

void vec2::operator+=(const vec2& p) {
    x += p.x;
    y += p.y;
}
void vec2::operator-=(const vec2& p) {
    x -= p.x;
    y -= p.y;
}

void vec2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
}

void vec2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    sanitize();
}


vec2::operator SDL_FPoint() const { // NOLINT(*-explicit-constructor)
    return {x, y};
}

gan::str_view & vec2::to_string(gan::str_view &buffer) const {
    return buffer << "FPos2D{x: " << (x) << ", y: " << y << '}';
}