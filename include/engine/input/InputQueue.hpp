#pragma once

#include "engine/input/InputConstants.hpp"
#include <vector>

#include "engine/gengine-globals/scene.hpp"

namespace gengine {
    class InputQueue {
    private:
        std::vector<std::pair<GENG_Input, short>> queue;
        int front = 0, back = 0;
    public:
        InputQueue() {queue.resize(30); }

        void push(GENG_Input input) {
            if ((back + 1) % queue.size() != front)
                queue[back++] = std::pair(input, GENG_buffer);
            back %= queue.size();
        }
        GENG_Input pop() {
            GENG_Input returnVal = GENG_Input::NONE;
            if (front != back)
                returnVal = queue[front++].first;
            front %= queue.size();
            return returnVal;
        }

        bool empty() {
            return front == back;
        }
        void update() {
            for (int f = front; f != back; f = (f + 1) % queue.size()) {
                queue[f].second -= (glb::scene.dt); // NOLINT(*-narrowing-conversions)
                if (queue[f].second <= 0) pop();
            }
        }
        void clear() {
            front = 0; back = 0;
        }
    };
}
