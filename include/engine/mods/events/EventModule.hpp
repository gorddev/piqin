#pragma once
#include <deque>
#include <functional>
#include <utility>

#include "engine/mods/Module.hpp"

namespace gan {

    /** @details The event manager has three primary event queues that all do different things. Note that a duration of -1 in the Epoch Queue specifies an indefinite time.
     * 1. @code Relative Queue@endcode › Events in this queue will only start decreasing in duration once the member in front of them has finished their task.
     * 2. @code Absolute Events@endcode › Putting an event into the absolute queue means that it will execute in the time you specify in ms regardless of other elements.
     * 3. @code Epoch Queue@endcode › Events in the epoch queue stop all other events from running until either 1: All of their durations expire, or 2. You call clear_epoch() on the EventManager.
     * */
    class EventManager final : public Module{
    private:
        std::deque<std::pair<double, std::function<void()>>> relative_queue;
        std::vector<std::pair<double, std::function<void()>>> absolute_queue;
        std::deque<std::pair<double, std::function<void()>>> epoch_queue;

    public:
        explicit EventManager(ModuleInit init);

        /// Updates all events in the EventModule
        void pre_update() override;

        /// Adds a relative event to the event queue, which is only called when all other events
        /// added before are completed (FIFO)
        /// @param duration When it is the turn for this event, this is the time is takes to call the event
        /// @param func Function you want to be called after all previous relative_events expire.
        void add_relative_event(double duration, std::function<void()> func);

        /// Adds an absolute event to the event queue, which occurs once it's timer is complete
        /// and there are no events in the Epoch Queue.
        /// @param duration Time until the provided function is called
        /// @param func Function you want to be called once its timer expires
        void add_absolute_event(double duration, std::function<void()> func);

        /// Adds an epoch event to the event queue, which blocks ALL other events from occuring
        /// until it is complete. Clear the epoch queue with @code clear_epoch@endcode.
        /// @param duration Duration of each event in the epoch queue. Acts like a relative queue
        /// @param func Function you want to be called once the event expires.
        void add_epoch_event(double duration, std::function<void()> func);

        /// Clears the epoch_queue and resumes the countdowns of all other events
        void clear_epoch();
    };
}
