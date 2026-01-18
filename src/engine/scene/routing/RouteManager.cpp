#include "engine/scene/routes/RouteManager.hpp"

using namespace gan;

void RouteManager::add_route(Route *p) {
    paths.push_back(p);
}

void RouteManager::add_route(Route* p, Gear* g, const FPos2D &offset) {
    p->set_target(g->t.pos + offset);
    add_route(p);
}

void RouteManager::remove_path(const Gear* g) {
    for (auto& p: paths) {
        if (p->get_payload() == g) {
            std::swap(p, paths.back());
            delete paths.back();
            paths.pop_back();
            break;
        }
    }
}

void RouteManager::remove_path(const Route* path) {
    for (auto& p: paths) {
        if (p == path) {
            std::swap(p, paths.back());
            delete paths.back();
            paths.pop_back();
            break;
        }
    }
}

void RouteManager::update() {
    for (auto it = paths.begin(); it != paths.end(); ) {
        Route* p = (*it);
        if ((*it)->update(scene.state)) {
            // Guarentees we hit the target at the end
            p->get_payload()->t.pos = p->get_target();
            // Then we destroy the path.
            delete p;
            it = paths.erase(it);
            continue;
        }
        it++;
    }
}

RouteManager::~RouteManager() {
    for (auto& p: paths) {
        delete p;
    }
}

