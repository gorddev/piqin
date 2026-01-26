#pragma once

#include "engine/types/strings/str_view/str_view.hpp"
#include "engine/types/positioning/dim2.hpp"
#include "engine/types/positioning/vec2.hpp"
#include "engine/types/positioning/pos2.hpp"

namespace gan {

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
        dim2 dim;
        /// Lets the renderer know if width or height have been changed.
        friend class Renderer;
        friend class RenderBuffer;
    public:
        /** @brief Camera position. */
        vec2 pos;
        /** @brief Camera offset */
        vec2 offset;

        /** @brief Default constructor. Sets position to (0, 0) and size to (100, 100). */
        Camera() : dim(320, 320), pos(0, 0) {  }

        /** @brief Construct camera with position and size.*/
        Camera(int x, int y, int w, int h) : dim(w, h), pos(x, y) { }

        /// @brief Get camera width. @return Width
        [[nodiscard]] short get_width() const { return dim.w; }

        /** @brief Get camera height. @return Height */
        [[nodiscard]] short get_height() const { return dim.h; }

        /** @brief Get the center point of the camera. @return Center position */
        [[nodiscard]] pos2 center() const {
            return pos2(pos.x + (dim.w / 2.0), pos.y - (dim.h / 2.0));
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
        void set_dimensions(dim2 dimensions) {
            dim = dimensions;
        }


        [[nodiscard]] dim2 get_dimensions() const {
            return dim;
        }

        /**
         * @brief Convert camera to string.
         * @return String with position and size
         */
        void to_fstring(gan::str_view& buffer) const {
            buffer << "x: " << pos.x << " y: " << pos.y
               << " w: " << dim.w << " h: " << dim.h;
        }

        void move_to_target(vec2 target) {
            pos += (target - pos)*0.2;
        }
    };
}