#pragma once
#include "engine/types/strings/str_view/str_view.hpp"


namespace gan {
    /**
     * @brief The Vertex is the fundamental coordinate system of the engine. @code x@endcode and @code y@endcode
     * determine position on the screen, and @code z@endcode determines rendering order.
     * @details The Vertex has many nice features that makes it easy to work with.
     * It has all the standard operator overloads you would expect, and some nice ones like @code %@endcode and @code %=@endcode.
     * Vertex also has a plethora of convenient utility functions:
     * - @code set(x,y,z)@endcode › Sets the coordinates of the Vertex
     * - @code unit()@endcode › Returns the unit vector of this veretx
     * - @code mag()@endcode › Returns the absolute magnitude of the Vertex
     * - @code xy_mag()@endcode › Returns the magnitude of the x-y coordinates of the Vertex
     * - @code dist(Vertex v)@endcode › Returns the distance from one Vertex to another.
     * - @code to_zero()@endcode › Turns all entries in the Vertex to zero
     * - @code is_zero()@endcode › Returns true if the Vertex's magnitude is zero
     * - @code randomize(std::pair range)@endcode › Randomizes all elements in teh Vertex in accordance with the range
     * @note There exists three to_string functions. Two are overloads for @code +@endcode and @code <<@endcode, and there also exists a generic @code to_string()@endcode.
     * @warning The == comparison for Vertexes only compares to the fourth decimal place.
     */
    struct vec3 {
        float x = 0;
        float y = 0;
        float z = 0;

        // Constructor for position
        vec3() = default;
        /// Z Defaults to 0 in this constructor
        vec3(const float x, const float y) : x(x), y(y), z(0) {}
        vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

        // Member functions
        /// Sets each coordinate of the vector
        void set(float xpos, float ypos, float zpos);
        /// Calculates the unit vector of this FPos3D
        [[nodiscard]] vec3 unit() const;
        /// Finds the magnitude of this FPos3D
        [[nodiscard]] float mag() const;
        /// Finds the magnitude of the x and y coordinates together
        [[nodiscard]] float xy_mag() const;
        /// Finds the distance of this FPos3D to another
        [[nodiscard]] float dist(vec3 v) const;
        /// Turns all values in the FPos3D to 0.f
        void to_zero();
        /// Resturns true if a FPos3D is zero
        [[nodiscard]] bool is_zero() const;
        /// Randomizes the values in a FPos3D. Specify a range from low to high. If no range is specified, randomness will be from -1 to 1.
        void randomize(std::pair<float, float> range = std::make_pair(0, 0));
        /// Gets the absolute value of this FPos3D
        vec3 abs();

        // Operator overloads for Vertexes
        // Operations with other Vertexes
        vec3 operator+(vec3 other) const;   /// Adds coordinates of two Vertices together
        vec3 operator-(vec3 other) const;   /// Subtracts coordinates of two vectors
        vec3 operator*(float scalar) const;   /// Multiplication of all coordinates
        vec3 operator/(float scalar) const;   /// Division via scalar of FPos3D
        vec3 operator%(int scalar) const;     /// Mod each element of the FPos3D by the scalar (turn to int)
        // Self operations
        void operator+=(vec3 other);  /// Addition + assignment
        void operator-=(vec3 other);  /// Subtraction + assignment
        void operator*=(float scalar);  /// Scalar multiplication + assignment
        void operator/=(float scalar);  /// Divison + assignment -> turn to 0 if dividing by 0.
        void operator%=(int scalar);    /// Modulus + assignment

        float operator*(vec3 other);  /// Dot product of two Vertexes

        float& operator[](int index);   /// Element wise access: [0]: x, [1]: y, [2]: z, [else]: x + error.
        void operator=(vec3 other); /// Copy constructor but with = sign. // NOLINT(*-unconventional-assign-operator)

        // To string methods below
        /// Add a FPos3D to a string
        std::string operator+(std::string s) const;
        /// Pipe a FPos3D with a string
        std::string operator<<(std::string s) const;
        /// Converts a FPos3D to a string.
        [[nodiscard]] gan::str_view &to_fstring(str_view &buffer) const;
        /// Checks to the fourth decimal if two Vertexes are equal.
        bool operator==(vec3 other) const;

        bool is_finite() const;

        void sanitize();
    };
}
