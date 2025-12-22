#pragma once
#include <SDL_rect.h>
#include <string>

namespace geng {

    struct FPos2D {
        float x, y;

        /// Default constructs to 0, 0
        FPos2D() : x(0), y(0) {}
        /// Can specify a construction.
        FPos2D(const float x, const float y) : x(x), y(y) {}

        // <><><> Calculations <><><>
        /// Turns it into a unit vector.
        [[nodiscard]] FPos2D unit();
        /// Find the magnitude of the point
        [[nodiscard]] float mag() const;
        /// Finds the distance to another FPos2D
        [[nodiscard]] float dist(FPos2D p) const;
        /// Gets the absolute value FPos2D of this FPos2D
        [[nodiscard]] FPos2D abs();

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
        bool operator==(const FPos2D& p) const;
        /// Inequality
        bool operator!=(const FPos2D& p) const;
        /// Addition by FPos2D
        FPos2D operator+(const FPos2D& p) const;
        /// Subtraction by FPos2D
        FPos2D operator-(const FPos2D& p) const;
        /// Multiplication by scalar
        FPos2D operator*(float scalar) const;
        /// Division by scalar
        FPos2D operator/(float scalar) const;
        // Assignment
        /// Self Addition
        void operator+=(const FPos2D& p);
        /// Self Subtraction
        void operator-=(const FPos2D& p);
        /// Self Multiplication
        void operator*=(float scalar);
        /// Self Division
        void operator/=(float scalar);
        /// Conversion to SDL_FPoint
        // ReSharper disable once CppNonExplicitConversionOperator
        operator SDL_FPoint() const; // NOLINT(*-explicit-constructor)

        /// To-String function
        std::string to_string() const;

    };
    
}
