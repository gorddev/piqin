#pragma once


template<typename T, typename... Args>
T* geng::Engine::create_layer(Args &&... args) {
    static_assert(std::is_base_of_v<Layer, T>);
    // Creates our layer initializer
    glog::warn << "before making layer initializer" << glog::endlog;
    LayerInit init(core, camera, texreg);
    glog::warn << "after making layer itself" << glog::endlog;

    // Creates the layer from our arguments
    T* layer = new T(init, std::forward<Args>(args)...);
    // Adds the layer to the layer map.
    glog::note.src("Engine::create_layer") << "Adding layer to engine.";
    layers.add_layer(layer);
    // Returns the layer for the user to use
    return layer;
}

template<typename T, typename... Args>
T* geng::Engine::create_router(Args &&... args) {
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


