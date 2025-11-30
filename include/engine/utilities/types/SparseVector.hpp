#pragma once

#include <vector>
#include <queue>

// Vector but when we erase we replace with nullptr
// Keep track of open positions with a queue.
namespace gengine::utils {
    template <typename T>
    struct SparseVector {
    private:
        std::vector<T*> entries;
        std::queue<short> positions;
    public:
        SparseVector() {}
        SparseVector(short size) {
            entries.resize(size);
        }
        // Returns index where it added the element
        int add(T* t) {
            short i = entries.size();
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
                    delete t;
                    entries[i] = nullptr;
                    positions.push(i);
                    return;
                }
            }
        }
        void erase(int i) {
            if (i < entries.size()) {
                delete entries[i];
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