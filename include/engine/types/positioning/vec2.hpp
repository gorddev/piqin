#pragma once
#include <SDL_rect.h>

#include "engine/types/strings/str_view/str_view.hpp"


namespace gan {

    struct vec2 {
        float x, y;

        /// Default constructs to 0, 0
        vec2() : x(0), y(0) {}
        /// Can specify a construction.
        vec2(const float x, const float y) : x(x), y(y) {}

        // <><><> Calculations <><><>
        /// Turns it into a unit vector.
        [[nodiscard]] vec2 unit();
        /// Find the magnitude of the point
        [[nodiscard]] float mag() const;
        /// Finds the distance to another FPos2D
        [[nodiscard]] float dist(vec2 p) const;
        /// Gets the absolute value FPos2D of this FPos2D
        [[nodiscard]] vec2 abs();

        // <><><> Modifiers <><><>
        /// Turns all the values in the FPos2D to 0
        void to_zero();
        /// Randomizes the values in the FPos2D
        void randomize();
        /// Converts to 0.f if the values are nAn
        void sanitize();
        /// sets the values in the object to the specified ones
        void set(float new_x, float new_y);

        // <><><> Verification <><><>
        /// Returns true if the FPos2D is 0
        [[nodiscard]] bool is_zero() const;
        /// Returns true if the values are finite
        [[nodiscard]] bool is_finite() const;

        // ********************************
        // <><><> Operator Overloads <><><>
        // ********************************
        // Calculation
        /// Equality
        bool operator==(const vec2& p) const;
        /// Inequality
        bool operator!=(const vec2& p) const;
        /// Addition by FPos2D
        vec2 operator+(const vec2& p) const;
        /// Subtraction by FPos2D
        vec2 operator-(const vec2& p) const;
        /// Multiplication by scalar
        vec2 operator*(float scalar) const;
        /// Division by scalar
        vec2 operator/(float scalar) const;
        // Assignment
        /// Self Addition
        void operator+=(const vec2& p);
        /// Self Subtraction
        void operator-=(const vec2& p);
        /// Self Multiplication
        void operator*=(float scalar);
        /// Self Division
        void operator/=(float scalar);
        /// Conversion to SDL_FPoint
        // ReSharper disable once CppNonExplicitConversionOperator
        operator SDL_FPoint() const; // NOLINT(*-explicit-constructor)

        /// To-String function
        gan::str_view& to_string(gan::str_view& buffer) const;

    };
    
}
