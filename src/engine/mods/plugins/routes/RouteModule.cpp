#include "engine/mods/plugins/routes/RouteModule.hpp"

using namespace gan;
using namespace gfx;

void RouteModule::add_route(Route *p) {
    // Removes the path if it already exists
    if (has_route(p->get_payload()))
        remove_path(p->get_payload());
    // pushes back the path to the end of the stack.
    paths.push_back(p);
    gear_to_routes[p->get_payload()] = p;
}

void RouteModule::add_route(Route* p, Gear* g, const vec2 &offset) {
    p->set_target(g->t.pos + offset);
    add_route(p);
}

void RouteModule::remove_path(Gear* g) {
    if (!has_route(g)) return;
    gear_to_routes.erase(g);
    for (size_t i = 0; i < paths.size(); ++i) {
        if (paths[i]->get_payload() == g) {
            std::swap(paths[i], paths.back());
            delete paths.back();
            paths.pop_back();
            break;
        }
    }
}

void RouteModule::remove_path(const Route* path) {
    for (auto& p: paths) {
        if (p == path) {
            std::swap(p, paths.back());
            gear_to_routes.erase(paths.back()->get_payload());
            delete paths.back();
            paths.pop_back();
            break;
        }
    }
}

bool RouteModule::has_route(Gear *g) {
    return gear_to_routes.contains(g);
}

void RouteModule::update() {
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

RouteModule::~RouteModule() {
    for (auto& p: paths) {
        delete p;
    }
}

