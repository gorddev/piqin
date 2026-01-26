#pragma once

#include "Banner.hpp"
#include "engine/mods/Module.hpp"

namespace gan {

    /** Manages all the banners and makes sure that they get updated and shit **/
    class BannerModule : public Module{
    private:
        std::vector<Banner*> banners;
    public:
        /// Default constructor
        explicit BannerModule(ModuleInit init)
            : Module(init) {}

        /// Updates all banners
        void update() override {
            for (auto &i : banners)
                i->update(scene.state);
        }

        /// Adds a banner to the banner manager for updating
        void add_banner(Banner* banner) {
            banners.push_back(banner);
        }
        /// Removes a banner from the banner manager for updating.
        void remove_banner(const Banner* banner) {
            for (auto it = banners.begin(); it != banners.end(); ++it) {
                if (*it == banner) {
                    banners.erase(it);
                    return;
                }
            }
        }
    };

    using Banners = BannerModule;
}
