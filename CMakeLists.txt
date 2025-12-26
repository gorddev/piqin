cmake_minimum_required(VERSION 3.13)
project(norton)

set(CMAKE_CXX_STANDARD 20)

# These flags get passed when using emcmake
set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(norton
        src/main.cpp
        src/testing/Card.cpp
        src/engine/Engine.cpp
        #[[
        # Main and engine


        # Game Logic Below
        src/game/GameMaster.cpp
        # Blackjack
        # Player and hand
        src/game/blackjack/Board.cpp
        src/game/blackjack/Round.cpp
        src/game/blackjack/bj-objects/Hand.cpp
        # Cards
        src/game/blackjack/cards/Card.cpp
        src/game/blackjack/bj-objects/CardSelector.cpp
        src/game/blackjack/bj-objects/Draw.cpp
        src/game/blackjack/bj-objects/Deck.cpp
        src/game/blackjack/bj-objects/Discard.cpp
        # Logic
        src/game/blackjack/bj-objects/Pather.cpp
        src/game/blackjack/cards/CardLogic.cpp
        src/game/blackjack/bj-logic/BJ-Actions.cpp
        src/game/blackjack/bj-logic/BJ-Input.cpp
        src/game/blackjack/bj-logic/BJ-CardLogic.cpp
        ]]

        # Game Engine Below

        # Initialization
        src/engine/scene/initializer/Initializer.cpp
        src/engine/scene/initializer/TextureRegister.cpp

        # Events
        src/engine/scene/events/EventManager.cpp

        # Input
        src/engine/input/InputDistributor.cpp
        src/engine/input/InputHandler.cpp

        # Layering
        src/engine/layers/Layer.cpp
        src/engine/layers/LayerContext.cpp
        src/engine/layers/LayerManager.cpp

        # Sprites
        src/engine/scene/sprites/SpriteManager.cpp
        src/engine/scene/sprites/Sprite.cpp

        # Banners
        src/engine/scene/banners/Banner.cpp
        src/engine/scene/banners/BannerBuffer.cpp
        src/engine/scene/banners/text/Text.cpp
        src/engine/scene/banners/text/TextParser.cpp

        # Fonts
        src/engine/scene/font/Font.cpp

        # Effects
        src/engine/scene/morphs/MorphManager.cpp
        src/engine/scene/morphs/morph-types/Shake.cpp
        src/engine/scene/morphs/morph-types/Stretch.cpp

        # Particles
        src/engine/scene/particles/ParticleManager.cpp
        src/engine/scene/particles/particle-types/Rhombus.cpp
        src/engine/scene/particles/particle-types/Sparkle.cpp

        # Physics
        src/engine/scene/physics/PhysicsMaestro.cpp

        # Rendering
        src/engine/render/Renderer.cpp
        src/engine/render/Background.cpp
        src/engine/render/shadows/ShadowBank.cpp
        src/engine/render/RenderBuffer.cpp

        # Routing
        src/engine/scene/routing/RouteManager.cpp
        src/engine/scene/routing/route-types/Balloon.cpp
        src/engine/scene/routing/route-types/Torpedo.cpp
        src/engine/scene/routing/route-types/Rebound.cpp
        src/engine/scene/routing/route-types/Linear.cpp
        src/engine/scene/routing/route-types/Sine.cpp

        # Frame Management
        src/engine/scene/animation/FrameList.cpp
        src/engine/scene/animation/FrameTable.cpp
        src/engine/scene/animation/Frames.cpp
        src/engine/scene/animation/AnimInfo.cpp

        # Utilities
        src/engine/utilities/IMGDecoder.cpp
        src/engine/utilities/Utilities.cpp

        #Tilesets
        src/engine/scene/tilesets/Tileset.cpp

        # Types
        src/engine/types/positioning/FPos2D.cpp
        src/engine/types/Transform2D.cpp
        src/engine/types/AnimBox2D.cpp

        # Worlds
        src/engine/scene/world/GameWorld.cpp
        src/engine/scene/world/deserialization/WorldLoader.cpp

        # Debug
        src/engine/debug/DebugManager.cpp
        src/engine/debug/debug-utilities/DebugInfoBanner.cpp
)

target_include_directories(norton PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include/scene
    ${CMAKE_SOURCE_DIR}/src/scene
)

# Emscripten-specific compile + link flags
target_compile_options(norton PRIVATE 
    "-sUSE_SDL=2"
    )
# regular run

target_link_options(norton PRIVATE 
    "-sUSE_SDL=2"
    "--shell-file=${CMAKE_SOURCE_DIR}/assets/game_shell.html"
    "--preload-file=assets"
    "--emrun"
    "-O0"
    "-sMAXIMUM_MEMORY=256MB"
    "-sALLOW_MEMORY_GROWTH"
    "-sUSE_SDL_IMAGE=2"
    "-sSDL2_IMAGE_FORMATS=[\"png\"]"
    "-sUSE_WEBGL2=1"
    "-fwasm-exceptions"
    "-std=c++20"
)


# release build

#debug
#[[
target_link_options(norton PRIVATE
        "-sUSE_SDL=2"
        "--shell-file=${CMAKE_SOURCE_DIR}/assets/game_shell.html"
        "--preload-file=assets"
        "--emrun"
        "-sMAXIMUM_MEMORY=32MB"
        "-sALLOW_MEMORY_GROWTH"
        "-sASSERTIONS=1"
        "-sSTACK_OVERFLOW_CHECK=2"
        "-sSAFE_HEAP=1"
        "-g4"
        "-O0"
        "-gsource-map"
)
]]

set(CMAKE_TOOLCHAIN_FILE "/Users/gordie/emsdk/upstream/emscripten/.../Emscripten.cmake")
