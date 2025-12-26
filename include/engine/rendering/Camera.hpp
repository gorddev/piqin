#pragma once
#include <sstream>

#include "../debug/geng_debug.hpp"
#include "engine/types/strings/str_view/str_view.hpp"
#include "engine/types/positioning/Dim2D.hpp"
#include "engine/types/positioning/FPos2D.hpp"
#include "engine/types/positioning/Pos2D.hpp"

namespace geng {

    /**
     * @brief Camera that stores a position and size › Each @code Layer@endcode owns its own camera.
     *
     * @details
     * The Camera defines a rectangular view using a position and dimensions.
     * The @code RenderBuffer@endcode uses camera information to render things in the
     * correct place, as well as the Layer's @code InputHandler@endcode uses it to determine
     * mouse clicking and positioning.
     * - ---- Member variables:
     * - @code Pos2D pos@endcode : Camera position.
     * - @code Dim2D dim@endcode : Camera width and height (private).
     * - ---- Constructors:
     * - @code Camera()@endcode : Position (0, 0), size (100, 100).
     * - @code Camera(int x, int y, int w, int h)@endcode : Custom position and size.
     * - ---- Functions:
     * - @code get_width()@endcode : Get camera width.
     * - @code get_height()@endcode : Get camera height.
     * - @code center()@endcode : Get center point.
     * - @code set_width()@endcode : Set camera width.
     * - @code set_height()@endcode : Set camera height.
     * - @code to_string()@endcode : Get position and size as text.
     */
    class Camera {
    private:
        /** @brief Camera width and height. */
        Dim2D dim;

    public:
        /** @brief Camera position. */
        FPos2D pos;

        /** @brief Default constructor. Sets position to (0, 0) and size to (100, 100). */
        Camera() : pos(0, 0), dim(100, 100) {  }

        /** @brief Construct camera with position and size.*/
        Camera(int x, int y, int w, int h) : pos(x, y), dim(w, h) { }

        /// @brief Get camera width. @return Width
        [[nodiscard]] short get_width() const { return dim.w; }

        /** @brief Get camera height. @return Height */
        [[nodiscard]] short get_height() const { return dim.h; }

        /** @brief Get the center point of the camera. @return Center position */
        [[nodiscard]] Pos2D center() const {
            return Pos2D(pos.x + (dim.w / 2.0), pos.y - (dim.h / 2.0));
        }
        /** @brief Returns the bottom boundary of the camera. @return int bottom */
        [[nodiscard]] int bottom() const { return pos.y + dim.h; }
        /// @brief Returns the right boundary of the camera. @return int right */
        [[nodiscard]] int right() const { return pos.x + dim.w; }

        /**
         * @brief Set camera width.
         * @param w New width
         */
        void set_width(short w) { dim.w = w; }

        /**
         * @brief Set camera height.
         * @param h New height
         */
        void set_height(short h) { dim.h = h; }

        /**
         * @brief Set camera's width and height.
         * @param Dim2D New Dimensions
         */
        void set_dimensions(Dim2D dimensions) {
            dim = dimensions;
        }

        [[nodiscard]] Dim2D get_dimensions() const {
            return dim;
        }

        /**
         * @brief Convert camera to string.
         * @return String with position and size
         */
        void to_fstring(geng::str_view& buffer) const {
            buffer << "x: " << pos.x << " y: " << pos.y
               << " w: " << dim.w << " h: " << dim.h;
        }
    };
}