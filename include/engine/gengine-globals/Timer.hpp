#pragma once
#include <functional>
#include <deque>
#include "scene.hpp"

namespace gengine {
    class EventTimer final {
    private:
        std::deque<std::pair<float, std::function<void()>>> callbacks;
    public:
        EventTimer() = default;

        void add_event(float time, const std::function<void()> callback) {
            callbacks.emplace_back(time, callback);
        }

        void update() {
            if (!callbacks.empty()) {
                callbacks.front().first -= glb::scene.dt;
                if (callbacks.front().first <= 0) {
                    const auto f = callbacks.front().second;
                    callbacks.pop_front();
                    f();
                }
            }
        }
    };

    inline EventTimer GENG_Events;
}
