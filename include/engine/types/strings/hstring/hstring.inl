#pragma once

namespace gan {

// ******************************
// Private helpers
// ******************************

inline void hstring::ensure(uint32_t needed) {
    if (needed <= cap) return;

    uint32_t new_cap = std::max(cap * 2u, needed);
    char* n = static_cast<char*>(::operator new(new_cap + 1));

    if (data != nullptr) {
        std::memcpy(n, data, len+1);
        ::operator delete(data);
    }

    data = n;
    cap  = new_cap;

    data[len] = '\0';
}

// ******************************
// Constructors / Destructor
// ******************************

inline hstring::hstring() {
    ensure(1); // reserve at least one for null
    len = 0;
    data[0] = '\0';  // always null-terminated
}

inline hstring::hstring(uint32_t reserve) {
    ensure(reserve);
    len = 0;
    data[0] = '\0';
}

template<uint32_t N>
inline hstring::hstring(const char (&str)[N]) {
    ensure(N);       // N includes space for '\0'
    std::memcpy(data, str, N - 1);
    len = N - 1;
    data[len] = '\0';
}

inline hstring::hstring(const char* str) {
    if (!str) {
        ensure(1);
        len = 0;
        data[0] = '\0';
        return;
    }
    auto n = static_cast<uint32_t>(std::strlen(str));
    ensure(n + 1);
    std::memcpy(data, str, n);
    len = n;
    data[len] = '\0';
}

inline hstring::hstring(const char* str, uint32_t count) {
    if (!str) {
        ensure(1);
        len = 0;
        data[0] = '\0';
        return;
    }
    ensure(count + 1);
    std::memcpy(data, str, count);
    len = count;
    data[len] = '\0';
}

inline hstring::hstring(const hstring& other) {
    ensure(other.len + 1);
    std::memcpy(data, other.data, other.len);
    len = other.len;
    data[len] = '\0';
    precision = other.precision;
}

inline hstring::hstring(hstring&& other) noexcept
    : data(other.data), len(other.len), cap(other.cap), precision(other.precision) {
    other.data = nullptr;
    other.len = 0;
    other.cap = 0;
}

inline hstring::~hstring() {
    ::operator delete(data);
}

// ******************************
// Assignment operators
// ******************************

inline hstring& hstring::operator=(const char text[]) {
    uint32_t str_len = strlen(text);
    uint32_t new_len = std::max(str_len, len);
    ensure(new_len+1);
    std::memcpy(data, text, str_len);
    len = new_len;
    data[len] = '\0';
    return *this;
}

inline hstring& hstring::operator=(const hstring& o) {
    if (this == &o) return *this;
    uint32_t new_len = std::max(o.len, len);
    ensure(new_len+1);
    std::memcpy(data, o.data, o.len);
    data[len] = '\0';
    precision = o.precision;
    return *this;
}

inline hstring& hstring::operator=(hstring&& o) noexcept {
    std::swap(data, o.data);
    std::swap(len, o.len);
    std::swap(cap, o.cap);
    std::swap(precision, o.precision);
    return *this;
}

// ******************************
// Utility
// ******************************

inline void hstring::resize(uint32_t new_cap) { ensure (new_cap + 1); }
inline uint32_t hstring::length() const { return len; }
inline uint32_t hstring::capacity() const { return cap; }

inline void hstring::clear() { len = 0; data[0] = '\0'; }
inline bool hstring::empty() const { return len == 0; }

inline char& hstring::operator[](uint32_t i) {
    if (i >= len) hstring_throw_err("hstring index OOB");
    return data[i];
}

inline const char& hstring::operator[](uint32_t i) const {
    if (i >= len) hstring_throw_err("hstring index OOB");
    return data[i];
}

// Always safe: cstr just returns pointer, buffer is always null-terminated
inline char* hstring::c_str() { return data; }
inline const char* hstring::c_str() const { return data; }
inline hstring::operator const char*() { return data; }
inline str_view hstring::wrap() { return {data, len, cap}; }

// ******************************
// Equality
// ******************************

template<uint32_t N>
inline bool hstring::operator==(const char (&str)[N]) const {
    if (len != N - 1) return false;
    return std::memcmp(data, str, len) == 0;
}

inline bool hstring::operator==(const char* str) const {
    if (!str) return len == 0;
    return std::strlen(str) == len && std::memcmp(data, str, len) == 0;
}

inline bool hstring::operator==(const hstring& o) const {
    return len == o.len && std::memcmp(data, o.data, len) == 0;
}

// ******************************
// Operator <<
// ******************************

inline hstring& hstring::operator<<(const char* str) {
    if (!str) return *this << "nullptr";
    auto n = static_cast<uint32_t>(std::strlen(str));
    ensure(len + n + 1);
    std::memcpy(data + len, str, n);
    len += n;
    data[len] = '\0';
    return *this;
}

template<uint32_t N>
inline hstring& hstring::operator<<(const char (&str)[N]) {
    ensure(len + (N - 1) + 1);
    std::memcpy(data + len, str, N - 1);
    len += N - 1;
    data[len] = '\0';
    return *this;
}

inline hstring& hstring::operator<<(char c) {
    ensure(len + 2);
    data[len++] = c;
    data[len] = '\0';
    return *this;
}

inline hstring& hstring::operator<<(int num) {
    ensure(len + 32 + 1);
    auto* start = data + len;
    auto res = std::to_chars(start, data + cap, num);
    len += static_cast<uint32_t>(res.ptr - start);
    data[len] = '\0';
    return *this;
}

inline hstring& hstring::operator<<(float num) {
    ensure(len + 32 + 1);
    auto* start = data + len;
    auto res = std::to_chars(start, data + cap, num, std::chars_format::fixed, precision);
    len += static_cast<uint32_t>(res.ptr - start);
    data[len] = '\0';
    return *this;
}

template<uint8_t P>
inline hstring& hstring::operator<<(gan::precision<P>) {
    precision = P;
    return *this;
}

inline hstring& hstring::operator<<(bool b) {
    return *this << (b ? "true" : "false");
}

inline hstring& hstring::operator<<(const void* ptr) {
    if (!ptr) return *this << "nullptr";
    ensure(len + 18 + 1);
    *this << "0x";
    auto* start = data + len;
    auto res = std::to_chars(start, data + cap, reinterpret_cast<uintptr_t>(ptr), 16);
    len += static_cast<uint32_t>(res.ptr - start);
    data[len] = '\0';
    return *this;
}

}