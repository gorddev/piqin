#pragma once
#include <concepts>

#include "engine/types/strings/fstring/fstring.hpp"

namespace gan {

    template<typename T>
    concept signed_int =
    std::signed_integral<T>;

    template<typename T>
    concept unsigned_int =
        std::unsigned_integral<T>;

    template<typename T>
    concept any_int =
        signed_int<T> || unsigned_int<T>;

    template<typename T>
    concept any_float =
        std::floating_point<T>;

    template<typename T>
    concept c_string =
        std::is_same_v<std::remove_cvref_t<T>, const char*> ||
        std::is_same_v<std::remove_cvref_t<T>, char*>;

    /// Clock used for logging
    inline auto start_time = std::chrono::steady_clock::now();


    /// Allows us to specify sources for our logs
    static const char gan_default_log_path[] = "";
    struct src {
        fstring<64> srcpath = gan_default_log_path;
        src(const char path[]) : srcpath(path) {}
    };

    enum class logcommands {
        ENDLOG = 0
    };
}
