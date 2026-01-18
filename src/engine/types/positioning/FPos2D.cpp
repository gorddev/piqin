#include "engine/types/positioning/FPos2D.hpp"

#include <cmath>

using namespace gan;

FPos2D FPos2D::unit() {
    float norm = sqrtf(x * x + y * y);
    FPos2D result(x/norm, y/norm);
    sanitize();
    return result;
}

float FPos2D::mag() const {
    return sqrtf(x * x + y * y);
}

float FPos2D::dist(FPos2D p) const {
    return sqrtf((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
}

FPos2D FPos2D::abs() {
    return {fabsf(x), fabsf(y)};
}

void FPos2D::to_zero() {
    x = 0; y = 0;
}

void FPos2D::randomize() {
    x = static_cast<float>(rand()) * ((random()%2) ? 1 : -1);
    y = static_cast<float>(rand()) * ((random()%2) ? 1 : -1);
}

void FPos2D::sanitize() {
    if (!std::isfinite(x)) x = 0.f;
    if (!std::isfinite(y)) y = 0.f;
}

void FPos2D::set(float new_x, float new_y) {
    x = new_x;
    y = new_y;
}

bool FPos2D::is_zero() const {
    return fabsf(x) < 1e-5 && fabsf(y) < 1e-5;
}

bool FPos2D::is_finite() const {
    return std::isfinite(x) && std::isfinite(y);
}

bool FPos2D::operator==(const FPos2D& p) const {
    return (fabsf(x - p.x) < 1e-6 && fabsf(y - p.y) < 1e-6);
}
bool FPos2D::operator!=(const FPos2D& p) const {
    return !(*this == p);
}

FPos2D FPos2D::operator+(const FPos2D& p) const {
    return {x + p.x, y + p.y};
}
FPos2D FPos2D::operator-(const FPos2D& p) const {
    return {x - p.x, y - p.y};
}

FPos2D FPos2D::operator*(const float scalar) const {
    return {x * scalar, y * scalar};
}

FPos2D FPos2D::operator/(float scalar) const {
    FPos2D result(x / scalar, y / scalar);
    result.sanitize();
    return result;
}

void FPos2D::operator+=(const FPos2D& p) {
    x += p.x;
    y += p.y;
}
void FPos2D::operator-=(const FPos2D& p) {
    x -= p.x;
    y -= p.y;
}

void FPos2D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
}

void FPos2D::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    sanitize();
}


FPos2D::operator SDL_FPoint() const { // NOLINT(*-explicit-constructor)
    return {x, y};
}

gan::str_view & FPos2D::to_string(gan::str_view &buffer) const {
    return buffer << "FPos2D{x: " << (x) << ", y: " << y << '}';
}