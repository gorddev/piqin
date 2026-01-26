#pragma once
#include "Trail.hpp"
#include "engine/mods/Module.hpp"

namespace gfx {

    class TrailModule final : public gan::Module {
    private:
        /// Vector containing each of the trails that will be attached to objects
        std::vector<Trail*> trails;

        /// Called whenever a gear is removed from the Engine
        /// so the TrailModule can handle it.
        void on_gear_removal(gan::Gear *) override;
    public:
        /// Creates the trail module
        explicit TrailModule(gan::ModuleInit init);

        /// Adds a trail to the trail module
        void add_trail(Trail *trail);

        /// Removes & deletes a trail from the trail module via trail pointer
        void remove_trail(const Trail* trail);
        /// Removes & deletes a trail from the trail module via gear pointer
        void remove_trail_from(const gan::Gear* g);

        /// Update function for the trail module
        void update() override;
    };
    /// Alternative name for the TrailModule for brevity purposes.
    using Trails = TrailModule;
}
