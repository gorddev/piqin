#pragma once

#include <charconv>
#include <stdexcept>

static void fstring_throw_err(const char err[]) {
    throw std::runtime_error(err);
}

// ****************************** -------------------
// Constructors
// ****************************** -------------------

template<uint32_t C>
template<uint16_t N>
constexpr geng::fstring<C>::fstring(const char (&str)[N]) { // NOLINT(*-explicit-constructor, *-pro-type-member-init)
    // Gets the lenght of our string
    len = std::min<uint32_t>(N - 1, C - 1);
    // Allocate memory for array
    std::copy(str, str + len, arr);
    // Assign our null character
    arr[len] = '\0';
    // Assign our precision identifier
    arr[C+1] = 10;
}
/// Constructor for constant character without length info
template<uint32_t C>
constexpr geng::fstring<C>::fstring(const char* str) { // NOLINT(*-pro-type-member-init)
    // If there's nothing there
    if (!str) {
        len = 0;
        arr[0] = '\0';
        return;
    }
    // Gets the lenght of our string
    len = std::min<uint32_t>(strlen(str), C - 1);
    // Allocate memory for array with builtin memcpy
    std::memcpy(arr, str, len);
    // Assign our null character
    arr[len] = '\0';
    // Assign our precision identifier
    arr[C+1] = 10;
}
/// Constructor for constant character with explicit length info
template<uint32_t C>
constexpr geng::fstring<C>::fstring(const char* str, const uint32_t length) { // NOLINT(*-pro-type-member-init)
    // If there's nothing there
    if (!str) {
        len = 0;
        arr[0] = '\0';
        return;
    }
    // Gets the lenght of our string
    len = std::min<uint32_t>(length, C - 1);
    // Allocate memory for array with builtin memcpy
    std::memcpy(arr, str, len);
    // Assign our null character
    arr[len] = '\0';
    // Assign our precision identifier
    arr[C+1] = 10;
}
/// Constructor for a singular character
template<uint32_t C>
constexpr geng::fstring<C>::fstring(char c) { // NOLINT(*-pro-type-member-init)
    // Length equals one
    len = 1;
    // Assign the first value
    arr[0] = c;
    arr[1] = '\0';
    // Assign our precision identifier
    arr[C+1] = 10;
}
/// Nothing in the constructor
template<uint32_t C>
constexpr geng::fstring<C>::fstring() { // NOLINT(*-pro-type-member-init)
    len = 0;
    arr[0] = '\0';
    // Assign our precision identifier
    arr[C+1] = 10;
}

// ****************************** -------------------
// Utility functions
// ****************************** -------------------

template<uint32_t C>
constexpr uint32_t geng::fstring<C>::length() const {
    return len;
}

template<uint32_t C>
constexpr uint32_t geng::fstring<C>::capacity() const {
    return C;
}

template<uint32_t C>
const char* geng::fstring<C>::cstr() {
    arr[len] = '\0';
    return &arr[0];
}

template<uint32_t C>
void geng::fstring<C>::clear() {
    len = 0;
}

template<uint32_t C>
geng::fstring<C> geng::fstring<C>::pop() {
    fstring<C> ret = *this;
    clear();
    return ret;
}

template<uint32_t C>
geng::str_view geng::fstring<C>::wrap() {
    return str_view(arr, len, C);
}


template<uint32_t C> template <uint32_t N>
void geng::fstring<C>::emplace(uint32_t index, const char (&str)[N])
    requires(index + N - 1 <= C)
{
    memcpy(arr + index, str, N - 1);
    if (index + (N - 1) > len) {
        len = index + (N - 1);
        arr[len] = '\0';
    }
}

template<uint32_t C>
void geng::fstring<C>::emplace(uint32_t index, const char* str, uint32_t count)
    requires(index + count <= C)
{
    if (!str) return; // nothing to copy
    std::memcpy(arr + index, str, count);
    // Update length if we extended past current len
    if (index + count > len) {
        len = index + count;
        arr[len] = '\0';
    }
}

template<uint32_t C>
template<uint32_t N>
constexpr geng::fstring<N> geng::fstring<C>::substr(uint32_t index) const {
    if (N > C - index)
        fstring_throw_err("fstring<C>::substr, access out of bounds memory.");
    return fstring<N>(arr + index, length);
}

// ****************************** -------------------
// Operator overloads
// ****************************** -------------------

template<uint32_t C>
constexpr char& geng::fstring<C>::operator[](uint32_t index) {
    if (index >= len)
        fstring_throw_err("fstring<C>::operator[], access out of bounds memory.");
    return arr[index];
}

//<><><><><>
// Equality tests
// <><><><><>

template<uint32_t C>
template<uint32_t N>
bool geng::fstring<C>::operator==(const char(&str)[N]) {
    if (len != N - 1) return false;
    return std::memcmp(arr, str, len) == 0;
}

template<uint32_t C>
bool geng::fstring<C>::operator==(const char *str) {
    for (int i = 0; i < len; i++) {
        if (arr[i] != str[i]) return false;
    }
    return str[len] == '\0';
}

template<uint32_t C>
template<uint32_t N>
geng::fstring<C>& geng::fstring<C>::operator<<(fstring<N> &other) {
    uint32_t bytes = std::min<uint32_t>( other.length(), C - len);
    memcpy(arr + len, other.cstr(), bytes);
    len += bytes;
    return *this;
}

template<uint32_t C>
template<uint32_t N>
geng::fstring<C> & geng::fstring<C>::operator<<(const char(&str)[N]) {
    uint32_t bytes = std::min<uint32_t>( N, C - len);
    memcpy(arr + len, str, bytes);
    len += bytes;
    return *this;
}

template<uint32_t C>
geng::fstring<C> & geng::fstring<C>::operator<<(const char *str) {
    if (!str) return (*this) << "null";
    uint32_t bytes = std::min<uint32_t>( strlen(str), C - len);
    memcpy(arr + len, str, bytes);
    len += bytes;
    return *this;
}

template<uint32_t C>
geng::fstring<C>& geng::fstring<C>::operator<<(const int num) {
    auto* start = arr + len;
    auto res = std::to_chars(start, arr + C, num);
    len += res.ptr - start;
    return *this;
}

template<uint32_t C>
template<uint8_t P>
geng::fstring<C> & geng::fstring<C>::operator<<(precision<P> precision) {
    arr[C + 1] = P;
    return *this;
}

template<uint32_t C>
geng::fstring<C> & geng::fstring<C>::operator<<(float num) {
    auto* start = arr + len;
    auto* end = arr + C;
    auto res = std::to_chars(start, end, num, std::chars_format::fixed, arr[C+1]);
    len += res.ptr - start;
    return *this;
}

template<uint32_t C>
geng::fstring<C> & geng::fstring<C>::operator<<(char c) {
    if (len != C - 1)
        arr[len++] = c;
    return *this;
}

template<uint32_t C>
geng::fstring<C>& geng::fstring<C>::operator<<(const void* ptr) {
    if (!ptr) return (*this) << "nullptr";
    auto addr = reinterpret_cast<uintptr_t>(ptr);
    *this << "0x";
    auto* start = arr + len;
    auto* end = arr + C;
    auto res = std::to_chars(start, end, addr, 16);
    len += res.ptr - start;
    return *this;
}

template<uint32_t C>
geng::fstring<C> & geng::fstring<C>::operator<<(bool b) {
    return *this << (b ? "true" : "false");
}

inline geng::str_view& geng::str_view::operator<<(const geng::str_view &other) {
    uint32_t bytes = std::min<uint32_t>( other.length(), cap - len);
    std::memmove(data + len, other.cstr(), bytes);
    len += bytes;
    return *this;
}

template<uint32_t N, uint32_t C>
geng::fstring<N+C> operator+(const geng::fstring<N> self, const geng::fstring<C>& other) {
    geng::fstring<N + C> ret;
    ret << self << other;
    return ret;
}

