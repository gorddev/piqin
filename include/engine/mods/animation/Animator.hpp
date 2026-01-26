#pragma once
#include <utility>

#include "engine/mods/Module.hpp"
#include "frames/FrameList.hpp"
#include "engine/mods/animation/sprites/SpriteManager.hpp"

namespace gan {

    class Animator : public Module {
    private:
        /// Manages all the sprites and their respective updating
        SpriteManager sprites;
        /// Manages the FrameTables for all sprites
        FrameList frame_tables;
    public:
        explicit Animator(const ModuleInit& init)
            : Module(init) {}

        /// Calls update on all the frameTables
        void update() override {
            sprites.update(scene);
        }

        /// Calls post_update on all the frameTables
        void post_update() override {
            sprites.update_frames(scene);
        }

        /// Adds sprite to the list of sprites
        void add_sprite(Sprite* s) {
            sprites.add_sprite(s);
            scene.gears.add_gear(s, module_id);
        }

        /// Adds several sprites to the SpriteManager and gear list
        template<typename T>
        requires(std::derived_from<T, Sprite>)
        void add_sprites(std::vector<T> sprite_list) {
            sprites.add_sprites(sprite_list);
            scene.gears.add_gear(sprite_list);
        }

        /// Generates a FrameTable for use by Sprites in the animator
        /// @param path Path to the texture used in the FrameTable
        /// @param ft FrameTable added to the Animator
        /// @return Integer id of the respective FrameTable
        int create_frametable(hstring path, FrameTable ft) {
            IMG_Info i = IMGDecoder::PNG_Info(path);
            ft._init(i);
            ft.set_texture_id(scene.tex_reg.register_texture(path));
            return frame_tables.add_table(std::move(ft));
        }

        /// Generates a FrameTable for use by Sprites in the animator
        /// @param path Path to the texture used in the FrameTable
        /// @param ft FrameTable added to the Animator
        /// @return Integer id of the respective FrameTable
        int gen_ft(hstring path, const FrameTable &ft) {
            return create_frametable(std::move(path), ft);
        }

        /// @param id ID of the FrameTable you want to access
        /// @return The FrameTable at the requested ID
        FrameTable& get_frame_table(const int id) {
            return frame_tables.get_table(id);
        }

        FrameTable& get_ft(const int id) {
            return frame_tables.get_table(id);
        }
    };

    /// Using for shorter syntax
    using Anim = Animator;
}
