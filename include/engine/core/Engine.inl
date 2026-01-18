#pragma once

template<typename T, typename... Args>
T* gan::Engine::create_layer(Args &&... args) {
    static_assert(std::is_base_of_v<Layer, T>);
    // Creates our layer initializer
    LayerInit init(core, texreg);

    // Creates the layer from our arguments
    T* layer = new T(init, std::forward<Args>(args)...);
    // Adds the layer to the layer map.
    glog::note.src("Engine::create_layer") << "Adding layer "
        << layer->get_name().c_str() << " to engine." << glog::endlog;
    // Adds layer to the layer manager
    layers.add_layer(layer);
    // Gives the layer access to the current keybinds
    input.emplace_key_states(layer);

    return layer;
}

template<typename T, typename... Args>
T* gan::Engine::create_router(Args &&... args) {
    static_assert(std::is_base_of_v<InputRouter, T>);
    // Creates our layer initializer
    InputRouterInit init(core, input.get_keystates());

    // Creates the layer from our arguments
    T* router = new T(init, std::forward<Args>(args)...);
    // Adds the layer to the layer map.
    input.add_input_router(router);
    // Returns the layer for the user to use
    return router;
}


