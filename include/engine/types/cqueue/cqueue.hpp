#pragma once
#include <cstddef>
#include <type_traits>
#include <iterator>

namespace gan {

    /**
     * @brief A fixed-size circular queue that automatically overwrites old elements when full.
     * @tparam T Type of elements stored in the queue.
     * @tparam N Maximum number of elements the queue can hold (must be > 0).
     * @note Primarily intended for tracking metrics like FPS, but can store any type.
     * Integrates with iterators for range-based for loops.
     */
    template<typename T, std::size_t N>
    class cqueue {
        static_assert(N > 0, "cqueue size must be > 0");

    public:
        /// Default constructor initializes an empty queue.
        constexpr cqueue() = default;

        /**
         * @brief Pushes a copy of an element into the queue.
         *        If the queue is full, the oldest element is overwritten.
         * @param value Element to push.
         */
        void push(const T& value) noexcept(std::is_nothrow_copy_assignable_v<T>) {
            buffer[tail] = value;
            advance();
        }

        /**
         * @brief Pushes a moved element into the queue.
         *        If the queue is full, the oldest element is overwritten.
         *
         * @param value Element to push (rvalue reference).
         */
        void push(T&& value) noexcept(std::is_nothrow_move_assignable_v<T>) {
            buffer[tail] = std::move(value);
            advance();
        }

        /**
         * @brief Pops the oldest element from the queue.
         *
         * @param out Reference to store the popped element.
         * @return true if an element was popped, false if the queue was empty.
         */
        bool pop(T& out) noexcept(std::is_nothrow_copy_assignable_v<T>) {
            if (empty())
                return false;

            out = buffer[head];
            head = (head + 1) % N;
            --count;
            return true;
        }

        /// @brief Access the front (oldest) element.
        T& front() noexcept { return buffer[head]; }
        /// @brief Access the front (oldest) element (const version).
        const T& front() const noexcept { return buffer[head]; }

        /// @brief Check if the queue is empty.
        bool empty() const noexcept { return count == 0; }

        /// @brief Check if the queue is full.
        bool full()  const noexcept { return count == N; }

        /// @brief Get the number of elements currently in the queue.
        std::size_t size() const noexcept { return count; }

        /// @brief Get the maximum capacity of the queue.
        constexpr std::size_t capacity() const noexcept { return N; }

        /// @brief Clear the queue, resetting head, tail, and count.
        void clear() noexcept {
            head = tail = count = 0;
        }

        /**
         * @brief Forward iterator for cqueue.
         *
         * Supports range-based for loops and STL-style iteration.
         */
        class iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            iterator(cqueue* q, std::size_t index, std::size_t remaining)
                : q(q), index(index), remaining(remaining) {}

            reference operator*() const { return q->buffer[index]; }
            pointer   operator->() const { return &q->buffer[index]; }

            iterator& operator++() {
                index = (index + 1) % N;
                --remaining;
                return *this;
            }

            bool operator==(const iterator& other) const {
                return remaining == other.remaining;
            }

            bool operator!=(const iterator& other) const {
                return !(*this == other);
            }

        private:
            cqueue* q;             ///< Pointer to the queue being iterated.
            std::size_t index;     ///< Current index in the buffer.
            std::size_t remaining; ///< Number of elements left to iterate.
        };

        /// @brief Returns an iterator pointing to the front (oldest element).
        iterator begin() {
            return iterator(this, head, count);
        }

        /// @brief Returns an iterator pointing past the last element.
        iterator end() {
            return iterator(this, tail, 0);
        }

    private:
        /**
         * @brief Advance tail and head (if full) after a push operation.
         * Increments tail to the next position, and moves head forward if the queue was full.
         */
        void advance() {
            if (full()) {
                head = (head + 1) % N;
            } else {
                ++count;
            }
            tail = (tail + 1) % N;
        }

        ///Fixed-size storage for queue elements.
        T buffer[N];
        std::size_t head = 0; ///< Index of the oldest element.
        std::size_t tail = 0; ///< Index of the next insertion.
        std::size_t count = 0; ///< Number of elements currently in the queue.
    };
}