#pragma once

#include "engine/debug/debug-utilities/DebugText.hpp"
#include "engine/rendering/Camera.hpp"
#include "engine/mods/banners/Banner.hpp"

namespace gan::debug {

    class ConsoleLogger : public Banner {
    private:
        std::vector<DebugText> text;
        int front = 0;
        EngineContext& core;
    public:
        ConsoleLogger(const Camera& cam, EngineContext& core) : Banner({t.w/2, 1.f*cam.get_height()}, 100, 100), core(core) {
            text.reserve(4);
            set_draggable();
        }

        void update() {

            if (glog::dev.logged) {
                if (text.size() < 4) {
                    text.emplace_back(DebugText(glog::dev.stream.c_str(), core.get_font(0)));
                    add_widget(&text.back());
                }
                else {
                    text[front].update_text(glog::dev.stream.c_str());
                }

                front = (front + 1) % 4;

                float prevheight = -100;
                for (int i = 0, j = (front + 3)%4; i < text.size(); i++, j++) {
                    prevheight -= text[j % 4].get_height();
                    text[j % 4].set_pos({0, static_cast<int>(prevheight)});
                }
                glog::dev.logged = false;
                glog::dev.clear();
            }
        }

        void notify_screen_resolution_change(dim2 new_resolution) {
            set_pos({t.w/2, 1.f*new_resolution.h});
        }
    };

}
