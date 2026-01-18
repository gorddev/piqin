cmake_minimum_required(VERSION 3.20)
project(norton)

set(CMAKE_CXX_STANDARD 20)

# Find SDL2 on your system
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

message(STATUS "SDL2_LIBRARIES=${SDL2_LIBRARIES}")
message(STATUS "SDL2_IMAGE_LIBRARIES=${SDL2_IMAGE_LIBRARIES}")
message(STATUS "SDL2_TTF_LIBRARIES=${SDL2_TTF_LIBRARIES}")

add_executable(norton
        src/main.cpp
        src/testing/Card.cpp
        src/engine/Engine.cpp
        src/engine/core/EngineContext.cpp

        # Game Engine Below
        src/engine/scene/initializer/Initializer.cpp
        src/engine/scene/initializer/TextureRegister.cpp

        src/engine/scene/events/EventManager.cpp

        src/engine/input/InputDistributor.cpp
        src/engine/input/InputHandler.cpp

        src/engine/layers/Layer.cpp
        src/engine/layers/LayerContext.cpp
        src/engine/layers/LayerManager.cpp

        src/engine/scene/sprites/SpriteManager.cpp
        src/engine/scene/sprites/Sprite.cpp

        src/engine/scene/banners/Banner.cpp
        src/engine/scene/banners/BannerBuffer.cpp
        src/engine/scene/banners/text/Text.cpp
        src/engine/scene/banners/text/TextParser.cpp

        src/engine/scene/font/Font.cpp

        src/engine/scene/morphs/MorphManager.cpp
        src/engine/scene/morphs/morph-types/Shake.cpp
        src/engine/scene/morphs/morph-types/Stretch.cpp

        src/engine/scene/particles/ParticleManager.cpp
        src/engine/scene/particles/particle-types/Rhombus.cpp
        src/engine/scene/particles/particle-types/RhombusRotate.cpp
        src/engine/scene/particles/particle-types/Sparkle.cpp

        src/engine/scene/physics/PhysicsMaestro.cpp

        src/engine/render/Renderer.cpp
        src/engine/render/Background.cpp
        src/engine/render/shadows/ShadowBank.cpp
        src/engine/render/RenderBuffer.cpp

        src/engine/scene/routing/RouteManager.cpp
        src/engine/scene/routing/route-types/Balloon.cpp
        src/engine/scene/routing/route-types/Torpedo.cpp
        src/engine/scene/routing/route-types/Rebound.cpp
        src/engine/scene/routing/route-types/Linear.cpp
        src/engine/scene/routing/route-types/Sine.cpp

        src/engine/scene/animation/FrameList.cpp
        src/engine/scene/animation/FrameTable.cpp
        src/engine/scene/animation/Frames.cpp
        src/engine/scene/animation/AnimInfo.cpp

        src/engine/utilities/IMGDecoder.cpp
        src/engine/utilities/Utilities.cpp

        src/engine/scene/tilesets/Tileset.cpp

        src/engine/types/positioning/FPos2D.cpp
        src/engine/types/Transform2D.cpp
        src/engine/types/AnimBox2D.cpp

        src/engine/scene/world/GameWorld.cpp
        src/engine/scene/world/deserialization/WorldLoader.cpp

        src/engine/debug/Console.cpp
        src/engine/debug/debug-utilities/DebugInfoBanner.cpp
        include/game/initialization/engine_initialization.hpp
        src/game/initialization/engine_initialization.cpp
        include/game/initialization/card_game_initialization.hpp
        include/game/initialization/initialization_data/small_card_frame_table.hpp
        include/engine/gan_anim.hpp
        include/engine/gan_plugins.hpp
        src/game/initialization/layer_initialization.cpp
        include/game/initialization/initialization_defaults.hpp
        include/engine/gan_types.hpp
        include/engine/scene/particles/particle-types/RhombusRotateSpiral.hpp
        src/engine/scene/particles/particle-types/RhombusRotateSpiral.cpp
        include/engine/scene/particles/ParticleGenerator.hpp
        include/engine/scene/particles/ParticleBank.hpp
        include/engine/scene/particles/Particle.hpp
        include/engine/scene/particles/ParticleState.hpp
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
        -Wall
        -Wextra
        -fsanitize=address
        -fno-omit-frame-pointer
        -ASAN_OPTIONS=detect_leaks=1
)

target_link_options(norton PRIVATE -fsanitize=address)