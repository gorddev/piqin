#include "engine/layers/LayerManager.hpp"

#include <algorithm>

using namespace gan;

LayerManager::LayerManager(EngineContext& world)
    : engine_context(world) {}

/// Updates each of the layers
void LayerManager::update(double dt) {
    for (auto& l : layers) {
        if (l->scene.is_running())
            l->update(dt);
    }
}

/// Gets a layer based on layer name
Layer* LayerManager::get_layer(gan::fstring<10> layer_name)  {
    for (auto& l : layers) {
        if (l->scene.get_name() == layer_name.c_str())
            return l;
    }
    return nullptr;
}

/// Returns the active layer
Layer* LayerManager::get_active_layer() {
    if (active_id == -1)
        return nullptr;
    return layers[active_id];
}

/// Sets the active layer based on layer pointer
void LayerManager::set_active_layer(Layer* l) {
    set_active_layer(l->scene.get_name());
}

/// Sets the active layer based on layer name
void LayerManager::set_active_layer(gan::fstring<10> name) {
    if (get_active_layer() != nullptr)
        get_active_layer()->scene._engine_deflag(LayerFlag::active);

    for (int i = 0; i < (int)layers.size(); i++) {
        if (layers[i]->scene.get_name() == name.c_str()) {
            active_id = i;
            layers[i]->scene._engine_flag(LayerFlag::active);
            return;
        }
    }

    if (get_active_layer() != nullptr)
        get_active_layer()->scene._engine_flag(LayerFlag::active);
}

/// Adds a layer to the LayerManager
void LayerManager::add_layer(Layer* l) {
    if (!has_layer(l)) {
        l->scene.id = static_cast<int>(layers.size());
        layers.push_back(l);
        if (active_id == -1)
            set_active_layer(l);
    }
    else
        glog::warn.src("LayerManager::add_layer()") <<
            "Aready added layer" << l->scene.get_name().c_str() << " to scene." << glog::endlog;
}

/// Increments the current active layer by 1 (loops back to)
void LayerManager::increment_active_layer() {
    if (active_id != -1) {
        layers[active_id]->scene._engine_deflag(LayerFlag::active);
        active_id++;
        if (active_id == (int)layers.size())
            active_id = 0;
        layers[active_id]->scene._engine_flag(LayerFlag::active);
    }
}

/// Removes a layer from the LayerManager
void LayerManager::remove_layer(fstring<10> name) {
    bool was_active = false;
    bool found = false;

    // Remove it from the vector
    for (auto it = layers.begin(); it != layers.end(); ++it) {
        if ((*it)->scene.get_name() == name.c_str()) {
            if ((*it)->is_active())
                was_active = true;
            found = true;
            delete (*it);
            layers.erase(it);
            break;
        }
    }

    if (!found) {
        glog::warn.src("LayerManager::remove_layer()")
            << "Error: Layer " << name.c_str() << " not found."
            << glog::endlog;
        return;
    }

    // Now we need to reassign layer map ids because indexes have changed.
    for (int i = 0; i < (int)layers.size(); i++)
        layers[i]->scene.id = i;

    if (was_active && !layers.empty())
        set_active_layer(layers[0]);
}

/// Removes a layer from the LayerManager via pointer
void LayerManager::remove_layer(const Layer* l) {
    remove_layer(l->scene.get_name());
}

/// Moves layer to front of order
void LayerManager::move_to_front(Layer* l) {
    auto it = std::find(layers.begin(), layers.end(), l);
    if (it == layers.end() || it == layers.end() - 1)
        return;

    int old_index = it - layers.begin();

    layers.erase(it);
    layers.push_back(l);

    // Shift everything down
    for (int i = old_index; i < (int)layers.size(); ++i)
        layers[i]->scene.id = i;
}

/// Move a layer to the back (bottom of rendering order)
void LayerManager::move_to_back(Layer* l) {
    auto it = std::find(layers.begin(), layers.end(), l);
    if (it == layers.end() || it == layers.begin())
        return;

    int old_index = it - layers.begin();

    layers.erase(it);
    layers.insert(layers.begin(), l);

    // Shift everything up
    for (int i = 0; i <= old_index; ++i)
        layers[i]->scene.id = i;
}

/// Move a layer forward by one in rendering order
void LayerManager::move_forward(Layer* l) {
    auto it = std::find(layers.begin(), layers.end(), l);
    if (it == layers.end() || it == layers.end() - 1)
        return;

    Layer* a = *it;
    Layer* b = *(it + 1);

    std::iter_swap(it, it + 1);
    std::swap(a->scene.id, b->scene.id);
}

/// Move a layer backward by one in rendering order
void LayerManager::move_backward(Layer* l) {
    auto it = std::find(layers.begin(), layers.end(), l);
    if (it == layers.end() || it == layers.begin())
        return;

    Layer* a = *it;
    Layer* b = *(it - 1);

    std::iter_swap(it, it - 1);
    std::swap(a->scene.id, b->scene.id);
}

/// Gets the list of layers
gch::vector<Layer*>& LayerManager::get_layer_list() {
    return layers;
}

bool LayerManager::has_layer(Layer *l) {
    for (auto& o: layers)
        if (l == o) return true;
    return false;
}


bool LayerManager::has_layer(fstring<10> name) {
    for (auto& o: layers)
        if (o->scene.get_name() == name.c_str()) return true;
    return false;
}

