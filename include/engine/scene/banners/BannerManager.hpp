#pragma once

#include "Banner.hpp"
#include "engine/layers/LayerContext.hpp"

namespace geng {

    /** Manages all the banners and makes sure that they get updated and shit **/
    class BannerManager {
    private:
        gch::vector<Banner*> banners;
    public:
        /// Default constructor
        explicit BannerManager() = default;

        /// Updates all banners
        void update(LayerContext& scene) {
            for (auto &i : banners)
                i->update(scene.state);
        }

        /// Adds a banner to the banner manager for updating
        void add_banner(Banner* banner) {
            banners.push_back(banner);
        }
        /// Removes a banner from the banner manager for updating.
        void remove_banner(Banner* banner) {
            for (auto it = banners.begin(); it != banners.end(); ++it) {
                if (*it == banner) {
                    banners.erase(it);
                    return;
                }
            }
        }
    };
}
