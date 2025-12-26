#pragma once
#include <deque>
#include <functional>
#include <utility>

#include "engine/layers/LayerContext.hpp"
#include "engine/types/external/vector.hpp"

namespace geng {

    /** @details The event manager has three primary event queues that all do different things. Note that a duration of -1 in the Epoch Queue specifies an indefinite time.
     * 1. @code Relative Queue@endcode › Events in this queue will only start decreasing in duration once the member in front of them has finished their task.
     * 2. @code Absolute Events@endcode › Putting an event into the absolute queue means that it will execute in the time you specify in ms regardless of other elements.
     * 3. @code Epoch Queue@endcode › Events in the epoch queue stop all other events from running until either 1: All of their durations expire, or 2. You call clear_epoch() on the EventManager.
     * */
    class EventManager final {
    private:
        std::deque<std::pair<double, std::function<void()>>> relative_queue;
        gch::vector<std::pair<double, std::function<void()>>> absolute_queue;
        std::deque<std::pair<double, std::function<void()>>> epoch_queue;

        /// Contains the layer core for updating queues
        LayerContext scene;
    public:
        explicit EventManager(LayerContext& layer_context);

        /// Updates all events in the Layer
        void update();

        void add_relative_event(double duration, std::function<void()>& func);
        void add_absolute_event(double duration, std::function<void()> func);
        void add_epoch_event(double duration, std::function<void()> func);

        void clear_epoch();
    };
}
