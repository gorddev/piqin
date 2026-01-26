cmake_minimum_required(VERSION 3.20)
project(norton)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_COMPILER)

# Find SDL2 on your system
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

message(STATUS "SDL2_LIBRARIES=${SDL2_LIBRARIES}")
message(STATUS "SDL2_IMAGE_LIBRARIES=${SDL2_IMAGE_LIBRARIES}")
message(STATUS "SDL2_TTF_LIBRARIES=${SDL2_TTF_LIBRARIES}")


add_executable(norton
        src/main.cpp
        src/engine/Engine.cpp
        src/engine/core/EngineContext.cpp

        # Game Engine Below
        src/engine/scene/initializer/Initializer.cpp
        src/engine/scene/initializer/TextureRegister.cpp

        src/engine/mods/events/EventModule.cpp

        src/engine/input/InputDistributor.cpp
        src/engine/input/InputHandler.cpp

        src/engine/layers/Layer.cpp
        src/engine/layers/layer-subobjects/LayerCore.cpp
        src/engine/layers/LayerManager.cpp

        src/engine/mods/animation/sprites/SpriteManager.cpp
        src/engine/mods/animation/sprites/Sprite.cpp

        src/engine/mods/banners/Banner.cpp
        src/engine/mods/banners/BannerBuffer.cpp
        src/engine/mods/banners/text/Text.cpp
        src/engine/mods/banners/text/TextParser.cpp

        src/engine/core/font/Font.cpp

        src/engine/mods/plugins/morphs/MorphManager.cpp
        src/engine/mods/plugins/morphs/morph-types/Shake.cpp
        src/engine/mods/plugins/morphs/morph-types/Stretch.cpp

        src/engine/mods/particles/ParticleManager.cpp
        src/engine/mods/particles/particle-types/Rhombus.cpp
        src/engine/mods/particles/particle-types/RhombusRotate.cpp
        src/engine/mods/particles/particle-types/Sparkle.cpp

        src/engine/mods/plugins/platformer/physics/PhysicsMaestro.cpp

        src/engine/render/Renderer.cpp
        src/engine/render/Background.cpp
        src/engine/render/shadows/ShadowBank.cpp
        src/engine/render/RenderBuffer.cpp

        src/engine/mods/plugins/routes/RouteModule.cpp
        src/engine/mods/plugins/routes/route-types/Balloon.cpp
        src/engine/mods/plugins/routes/route-types/Torpedo.cpp
        src/engine/mods/plugins/routes/route-types/Rebound.cpp
        src/engine/mods/plugins/routes/route-types/Linear.cpp
        src/engine/mods/plugins/routes/route-types/Sine.cpp

        src/engine/mods/animation/frames/FrameList.cpp
        src/engine/mods/animation/frames/FrameTable.cpp
        src/engine/mods/animation/frames/Frame.cpp
        src/engine/mods/animation/sprites/AnimInfo.cpp

        src/engine/utilities/IMGDecoder.cpp
        src/engine/utilities/Utilities.cpp

        src/engine/mods/plugins/platformer/tilesets/Tileset.cpp

        src/engine/types/positioning/vec2.cpp
        src/engine/types/Transform2D.cpp
        src/engine/types/AnimBox2D.cpp

        src/engine/mods/plugins/platformer/world/GameWorld.cpp
        src/engine/mods/plugins/platformer/world/deserialization/WorldLoader.cpp

        # debugs
        src/engine/debug/Console.cpp
        src/engine/debug/debug-utilities/DebugInfoBanner.cpp
        include/game/initialization/engine_initialization.hpp
        src/game/initialization/engine_initialization.cpp
        include/game/initialization/card_game_initialization.hpp
        include/engine/gan_std.hpp
        include/engine/gan_plugins.hpp
        src/game/initialization/layer_initialization.cpp
        include/game/initialization/initialization_defaults.hpp
        include/engine/gan_types.hpp
        include/engine/scene/particles/particle-types/RhombusRotateSpiral.hpp
        src/engine/mods/particles/particle-types/RhombusRotateSpiral.cpp
        src/game/cards/Card.cpp
        src/game/board/Deck.cpp
        src/game/blackjack/BlackjackHand.cpp
        src/engine/mods/banners/banner-types/HoverBanner.cpp
        src/engine/layers/layer-subobjects/GearList.cpp
        src/engine/mods/ModuleManager.cpp
        src/engine/mods/plugins/trails/TrailModule.cpp
)

# Include directories
target_include_directories(norton PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include/scene
        ${CMAKE_SOURCE_DIR}/src/scene
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
)

# Link SDL2 libraries
target_link_libraries(norton PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
)

# Extra compiler flags if needed
target_compile_options(norton PRIVATE
        # allllll the warnings
        -Wall
        -Wextra
        -Wno-unused-function # no unused function warnings tho im so sick of them
        -Wno-unused-parameter
        # optimize for speed
        -O3
        # enable link-time optimization
        -flto
        # deletes
        -fdata-sections
        -ffunction-sections
        # Remove unwind / stack metadata
        -fno-rtti
        -fno-unwind-tables
        -fno-asynchronous-unwind-tables
        # Hide symbols by default (huge for macOS + dylibs)
        -fvisibility=hidden
        # enables runtime type identification
        -frtti

        #-fsanitize=address,undefined
        #-fno-omit-frame-pointer
        #-g
)

if (APPLE)
    target_link_options(norton PRIVATE
            # Required for LTO
            -flto
            # Dead-strip unused code
            -Wl,-dead_strip
            -Wl,-dead_strip_dylibs

            #-fsanitize=address,undefined
    )
else()
    target_link_options(norton PRIVATE
            # Required for LTO
            -flto
            # Remove unused sections
            -Wl,--gc-sections
            # Strip symbols
            -s
    )
endif()
