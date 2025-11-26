#pragma once

#include "input/InputConstants.hpp"
#include <vector>

#include "Constants.hpp"

#define INIT_INPUT_QUEUE_SIZE 30
class InputQueue {
private:
    std::vector<std::pair<short, short>> queue;
    int front = 0, back = 0;
public:
    InputQueue() {queue.resize(INIT_INPUT_QUEUE_SIZE); }

    void push(short input) {
        if ((back + 1) % queue.size() != front)
            queue[back++] = std::pair(input, input::buffer);
        back %= queue.size();
    }
    short pop() {
        uint8_t returnVal = 0;
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
            queue[f].second -= (scene::dt); // NOLINT(*-narrowing-conversions)
            if (queue[f].second <= 0) pop();
        }
    }
    void clear() {
        front = 0; back = 0;
    }
};

