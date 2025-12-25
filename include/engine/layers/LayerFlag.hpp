#pragma once
#include <cstdint>

namespace geng {
    /** @brief LayerFlags allows the engine and layer to behave differently based on flags set.
          * Flags avaliable are:
          * - @code visible@endcode
          * - @code active@endcode
          * - @code running@endcode
          * - @code initialized@endcode
          * - @code strict@endcode › If we have a strict rendering style in terms of order
          * - @code z_indexed@endcode › If rendering is z_indexed
          * - @code batched@endcode › If rendering is batched by texture (default)
          */
    enum class LayerFlag : uint8_t {
        none        = 0,
        visible     = 1 << 0,
        active      = 1 << 1,
        running     = 1 << 2,
        initialized = 1 << 3,
        strict      = 1 << 4,
        z_indexed   = 1 << 5,
        batched     = 1 << 6
    };

    constexpr LayerFlag operator|(LayerFlag a, LayerFlag b) {
        return static_cast<LayerFlag>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }

    constexpr LayerFlag operator&(LayerFlag a, LayerFlag b) {
        return static_cast<LayerFlag>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }

    constexpr LayerFlag& operator|=(LayerFlag& a, LayerFlag b) {
        a = static_cast<LayerFlag>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
        return a;
    }

    constexpr LayerFlag& operator&=(LayerFlag& a, LayerFlag b) {
        a = static_cast<LayerFlag>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
        return a;
    }

    constexpr LayerFlag LayerFlagMask =
        LayerFlag::visible     |
        LayerFlag::active      |
        LayerFlag::running     |
        LayerFlag::initialized |
        LayerFlag::strict      |
        LayerFlag::z_indexed   |
        LayerFlag::batched;

    constexpr LayerFlag operator~(LayerFlag f) noexcept {
        return static_cast<LayerFlag>(
            static_cast<uint8_t>(LayerFlagMask) &
            ~static_cast<uint8_t>(f)
        );
    }
}
