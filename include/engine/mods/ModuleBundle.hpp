#pragma once

namespace gan {

    /** @brief A @code ModuleBundle@endcode an empty struct
     * that stores default-constructed modules in a object. Reduces
     * overall syntax-load on the ModuleManager.
     * @details Define default-constructed modules via templating
     * the ModuleBundle.
     */
    template<typename... Mods>
    struct ModuleBundle final {};
}