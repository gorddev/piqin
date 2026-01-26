#pragma once
#include "engine/scene/physics/Collider.hpp"

#define DFL_POS Pos2D{20.f,0.f}
#define MAX_VEL 1
class PlayerTest : public gan::Collider{
protected:
    bool grounded = false;
    bool clicked = false;
public:
    explicit PlayerTest(gan::FrameTable& frames)
        : Collider(frames,{8,11}) {
        t = gan::Transform2D{{20.f, 0.f}, 31, 31};
        offset = {0, 9};
        anim.set_animation(1);
        grounded = false;
    }

    void update(gan::LayerState& state) override {
        if (!clicked) {
            if (state.is_held(SDL_SCANCODE_D)) {
                vel.x += 0.05*state.get_dt();

                unflipX();
                if (grounded)
                    anim.suggest_animation(1);
            }
            else if (state.is_held(SDL_SCANCODE_A)) {
                vel.x -= 0.05*state.get_dt();
                flipX();
                if (grounded)
                    anim.suggest_animation(1);
            }
            else {
                if (grounded)
                    anim.suggest_animation(0);
                vel.x *= 0.6;
            }
            if (!grounded && vel.y < 0) {
                anim.suggest_animation(4);
            }
            vel.y += 0.002*state.get_dt();
            if (vel.x > MAX_VEL) vel.x = MAX_VEL;
            if (vel.x < -MAX_VEL) vel.x = -MAX_VEL;
            if (vel.y > MAX_VEL) vel.y = MAX_VEL;
            if (t.pos.y > 160) {
                t.pos.y = 20;
                vel.y = 0;
            }
            t.pos += vel*state.get_dt()*0.1;
        }
        grounded = false;
    }

    void collide(gan::ProposedCollision &collision) override {
        t.pos = collision.newPos;
        if (collision.delta.y < 0)
            grounded = true;
    }

    void on_key_press(SDL_Scancode key) override {
        if (key == SDL_SCANCODE_A && vel.x > 0.1)
            anim.set_animation(2, true);
        else if (key == SDL_SCANCODE_D && vel.x < -0.1)
            anim.set_animation(2, true);
        else if (key == SDL_SCANCODE_SPACE &&  grounded) {
            vel.y = -1;
            anim.set_animation(3, true);
        }
    }

    void on_click(gan::Pos2D pos) override {
        clicked = true;
        vel.y = 0;
        vel.x = 0;
    }

    void on_click_release(gan::Pos2D pos) override {
        clicked = false;
    }
};
