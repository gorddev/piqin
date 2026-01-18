#include "engine/scene/events/EventManager.hpp"

using namespace gan;

EventManager::EventManager(LayerContext &layer_context) : scene(layer_context) {}

void EventManager::update() {
    double dt = scene.state.get_dt();

    // 1. epoch queue: consumes time
    while (dt > 0 && !epoch_queue.empty()) {
        auto& [t, f] = epoch_queue.front();
        double consume = std::min(t, dt);
        t -= consume;
        dt -= consume;

        if (t <= 0) {
            f();
            epoch_queue.pop_front();
        }
    }
    if (dt <= 0)
        return;

    // 2) absolute queue: tick once
    for (auto it = absolute_queue.begin(); it != absolute_queue.end(); ) {
        it->first -= scene.state.get_dt();
        if (it->first <= 0) {
            it->second();
            it = absolute_queue.erase(it);
        } else {
            ++it;
        }
    }

    // 3. relative queue, consume until all time is emty
    while (dt > 0 && !relative_queue.empty()) {
        auto& [t, f] = relative_queue.front();
        double consume = std::min(t, dt);
        t -= consume;
        dt -= consume;

        if (t <= 0) {
            f();
            relative_queue.pop_front();
        }
    }
}

void EventManager::add_relative_event(double duration, std::function<void()> &func) {
    relative_queue.emplace_back(duration, func);
}

void EventManager::add_absolute_event(double duration, std::function<void()> func) {
    absolute_queue.emplace_back(duration, func);
}

void EventManager::add_epoch_event(double duration, std::function<void()> func) {
    epoch_queue.emplace_back(duration, func);
}

void EventManager::clear_epoch() {
    epoch_queue.clear();
}


