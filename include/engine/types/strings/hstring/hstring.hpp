#pragma once

#include <cstdint>
#include <cstring>
#include <stdexcept>

#include "../str_view/str_view.hpp"

namespace gan {

    /// Throw an error for hstring
    static inline void hstring_throw_err(const char* err) {
        throw std::runtime_error(err);
    }

    class hstring {
    private:
        char*     data = nullptr;
        uint32_t  len  = 0;
        uint32_t  cap  = 0;
        /// We need to keep track of precision manually now
        uint8_t   precision = 10;

    private:
        /// ensures we have enough space
        void ensure(uint32_t needed);

    public:
        //******************************
        // constructors & destructors
        //******************************

        hstring();
        /// reserves a certain amount in an hstring
        explicit hstring(uint32_t reserve);

        template<uint32_t N>
        /// String literal is provided
        hstring(const char (&str)[N]);
        /// No information on length
        hstring(const char* str);
        /// if we are provided a count manually
        hstring(const char* str, uint32_t count);
        /// if we get another hstring
        hstring(const hstring& other);
        /// copy constructor stealing heap space
        hstring(hstring&& other) noexcept;
        ~hstring();

        /// set equal to other hstrings
        hstring& operator=(const hstring& other);
        /// steals the other hstring's heap space
        hstring& operator=(hstring&& other) noexcept;

        //*****************************
        // utilities
        //***************************

        /// returns the length of the hstring
        [[nodiscard]] uint32_t length() const;
        /// returns the capacity of the hstring
        [[nodiscard]] uint32_t capacity() const;
        /// returns whether the hstring is empty or not
        [[nodiscard]] bool empty() const;
        /// clears out the hstring's data
        void clear();

        /// non-const [] operator for hstring
        /// @param i index of retrieval
        /// @return reference to character at that index
        char& operator[](uint32_t i);

        /// const [] operator for hstring
        /// @param i index of retrieval
        /// @return reference to const char at that index
        const char& operator[](uint32_t i) const;

        /// returns the cstring variant of an hstring
        char* c_str();
        /// returns a const version without the null terminator
        const char* c_str() const;
        /// implicitly convert to a cstring
        operator const char*();

        /// turns the hstring into a str_view
        str_view wrap();

        // ***********
        // Equality
        // ***********

        /// Compares to a string literal
        template<uint32_t N>
        bool operator==(const char (&str)[N]) const;
        /// compares the hstring to a const char
        bool operator==(const char* str) const;
        /// compares this hstring to another hstring
        bool operator==(const hstring& other) const;

        // *************
        // pippping <<<<<
        // *************

        /// get another const char
        hstring& operator<<(const char* str);

        /// if we know the length
        template<uint32_t N>
        hstring& operator<<(const char (&str)[N]);

        /// singular character
        hstring& operator<<(char c);
        /// an integer
        hstring& operator<<(int num);
        /// a float
        hstring& operator<<(float num);
        /// a double
        hstring& operator<<(double num) { return *this << static_cast<float>(num); }

        /// generic types
        template<typename T>
        hstring& operator<<(T mystery) { return *this << static_cast<int>(mystery); }

        /// Truncation and the like
        template<uint8_t P>
        hstring& operator<<(gan::precision<P>);

        /// For bools
        hstring& operator<<(bool b);
        /// For pointers
        hstring& operator<<(const void* ptr);
    };

}

#include <functional>
namespace std {
    template<>
    struct hash<gan::hstring> {
        size_t operator()(const gan::hstring& s) const noexcept {
            size_t h;
            if constexpr (sizeof(size_t) == 8) {
                h = 14695981039346656037ull;
            } else {
                h = 2166136261u;
            }
            const char* data = s.c_str();
            for (size_t i = 0; i < s.length(); ++i) {
                h ^= static_cast<unsigned char>(data[i]);
                if constexpr (sizeof(size_t) == 8)
                    h *= 1099511628211ull;
                else
                    h *= 16777619u;
            }
            return h;
        }
    };
}

#include "hstring.inl"