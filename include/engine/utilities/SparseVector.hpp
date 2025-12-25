#pragma once

#include <queue>

// Vector but when we erase we replace with nullptr
// Keep track of open positions with a queue.
namespace gutils {
    template <typename T>
    struct SparseVector {
    private:
        gch::vector<T*> entries;
        std::queue<int> positions;
    public:
        SparseVector() = default;
        explicit SparseVector(int size) {
            entries.resize(size);
        }
        // Returns index where it added the element
        int add(T* t) {
            int i = entries.size();
            if (!positions.empty()) {
                i = positions.front();
                entries[i] = t;
                positions.pop();
                return i;
            }
            entries.push_back(t);
            return i;
        }
        T* front() {
            for (auto&t : entries) {
                if (t != nullptr)
                    return t;
            }
            return nullptr;
        }
        T* back() {
            for (int i = entries.size() - 1; i >= 0; --i) {
                if (entries[i]!= nullptr)
                    return entries[i];
            }
            return nullptr;
        }
        void erase(T* t) {
            for (int i = 0; i < entries.size(); ++i) {
                if (entries[i]== t) {
                    entries[i] = nullptr;
                    positions.push(i);
                    return;
                }
            }
        }
        void erase(int i) {
            if (i < entries.size()) {
                entries[i] = nullptr;
                positions.push(i);
            }
        }
        T*& operator[](int i) {
            return entries[i];
        }
        auto begin() {
            return entries.begin();
        }
        auto end() {
            return entries.end();
        }
        void reserve( int i) {
            entries.reserve(i);
        }
        int size() {
            return entries.size();
        }
    };
}