#pragma once
#include <string>
// Generic object types

// Vertex is just a 3-point vector with all the standard operations
// Behaves exactly as expected. 
struct Vertex {
    float x = 0;
    float y = 0;
    float z = 0;

    // Constructor for position
    Vertex() = default;
    Vertex(const float x, const float y) : x(x), y(y), z(0) {}
    Vertex(const float x, const float y, const float z) : x(x), y(y), z(z) {}

    // Member functions
    // Allows you to set the coordinates
    void set(int xpos, int ypos, int zpos);
    // Finds the unit vector of this vertex
    [[nodiscard]] Vertex unit() const;
    // Finds the magnitude of this Vertex (int)
    [[nodiscard]] float mag() const;
    // Finds the magnitude of the x and y coordinate
    [[nodiscard]] float xy_mag() const;
    // Finds the distance of this Vertex to another
    [[nodiscard]] float dist(Vertex v) const;
    // Finds if a vector is zero
    bool zero() const;
    // Randomizes the values in the vertex
    void randomize(int limit = 0);

    // Operator overloads for Vertexes
    // Operations with other Vertexes
    Vertex operator+(Vertex other) const;
    Vertex operator-(Vertex other) const;
    Vertex operator*(float scalar) const;
    Vertex operator/(float scalar) const;
    Vertex operator%(int scalar) const;
    // Self operations
    void operator+=(Vertex other);
    void operator-=(Vertex other);
    void operator*=(float scalar);
    void operator%=(int scalar);

    float operator*(Vertex other);

    void operator/=(float scalar);
    float& operator[](int index);
    void operator=(Vertex other); // NOLINT(*-unconventional-assign-operator)

    //Tostring
    std::string operator+(std::string s);

    std::string to_string() const;

    bool operator==(Vertex other) const;
};